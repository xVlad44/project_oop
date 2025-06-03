#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> // For simple feedback
#include <QDebug>      // For debugging output

MainWindow::MainWindow(ArtifactController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller) // Store the controller
    , m_artifactsModel(new QStringListModel(this)) // Initialize the model
{
    ui->setupUi(this);

    if (!m_controller) {
        // Handle error: controller is null. Maybe show a message and disable UI.
        QMessageBox::critical(this, "Error", "Controller not available. Application might not function correctly.");
        // You might want to disable UI elements here
        return;
    }

    ui->artifactsListView->setModel(m_artifactsModel);

    // Initial population of the list
    populateArtifactsList();

    // Connect signals to slots (if not using auto-connection via on_..._clicked naming convention)
    // connect(ui->pushButton_Add, &QPushButton::clicked, this, &MainWindow::on_pushButton_Add_clicked);
    // connect(ui->artifactsListView, &QListView::clicked, this, &MainWindow::on_artifactsListView_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    // m_artifactsModel is a child of MainWindow, so it will be deleted automatically.
    // m_controller is not owned by MainWindow, so it's not deleted here.
}

void MainWindow::populateArtifactsList() {
    if (!m_controller) return;

    QStringList artifactDisplayList;
    std::vector<ArcheologicalArtifact> artifacts = m_controller->getAllArtifacts();
    for (const auto& artifact : artifacts) {
        // Display ID and Name, or customize as needed
        artifactDisplayList.append(QString("%1: %2").arg(artifact.getId()).arg(artifact.getName()));
    }
    m_artifactsModel->setStringList(artifactDisplayList);
}

void MainWindow::clearInputFields() {
    ui->lineEdit_Id->clear();
    ui->lineEdit_Name->clear();
    ui->textEdit_Description->clear();
    ui->lineEdit_Material->clear();
    ui->dateEdit_DiscoveryDate->setDate(QDate::currentDate()); // Reset to current date or a default
    ui->lineEdit_Location->clear();
}

ArcheologicalArtifact MainWindow::getArtifactFromFields() const {
    QString id = ui->lineEdit_Id->text();
    QString name = ui->lineEdit_Name->text();
    QString description = ui->textEdit_Description->toPlainText();
    QString material = ui->lineEdit_Material->text();
    QDate discoveryDate = ui->dateEdit_DiscoveryDate->date();
    QString location = ui->lineEdit_Location->text();
    return ArcheologicalArtifact(id, name, description, material, discoveryDate, location);
}

void MainWindow::populateFieldsFromArtifact(const ArcheologicalArtifact& artifact) {
    ui->lineEdit_Id->setText(artifact.getId());
    ui->lineEdit_Name->setText(artifact.getName());
    ui->textEdit_Description->setPlainText(artifact.getDescription());
    ui->lineEdit_Material->setText(artifact.getMaterial());
    ui->dateEdit_DiscoveryDate->setDate(artifact.getDiscoveryDate());
    ui->lineEdit_Location->setText(artifact.getLocation());
}

void MainWindow::on_pushButton_Add_clicked()
{
    if (!m_controller) return;
    try {
        ArcheologicalArtifact artifact = getArtifactFromFields();
        if (artifact.getId().isEmpty() || artifact.getName().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "ID and Name cannot be empty.");
            return;
        }
        // TODO: Add more robust validation
        m_controller->addArtifact(artifact.getId(), artifact.getName(), artifact.getDescription(),
                                  artifact.getMaterial(), artifact.getDiscoveryDate(), artifact.getLocation());
        populateArtifactsList();
        clearInputFields();
        QMessageBox::information(this, "Success", "Artifact added.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add artifact: %1").arg(e.what()));
    }
}

void MainWindow::on_pushButton_Update_clicked()
{
    if (!m_controller) return;

    QModelIndexList selectedIndexes = ui->artifactsListView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select an artifact to update.");
        return;
    }
    // Assuming single selection, and the displayed text contains the ID first.
    // This is a bit fragile; a better way is to store full objects or IDs in the model.
    QString selectedText = m_artifactsModel->data(selectedIndexes.first(), Qt::DisplayRole).toString();
    QString originalId = selectedText.split(":").first().trimmed(); // Extract ID

    if (originalId.isEmpty()) {
         QMessageBox::warning(this, "Error", "Could not determine the original ID of the selected artifact.");
         return;
    }

    try {
        ArcheologicalArtifact updatedArtifact = getArtifactFromFields();
         if (updatedArtifact.getId().isEmpty() || updatedArtifact.getName().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "ID and Name cannot be empty for update.");
            return;
        }
        // The controller's updateArtifact needs the original ID to find the item if the ID itself can change.
        // Or, the controller's updateArtifact assumes the ID in updatedArtifact is the key to find.
        // For now, let's assume the controller's updateArtifact uses the ID from the new artifact data to find and update.
        // If the ID can change, the controller needs the originalId.
        m_controller->updateArtifact(originalId, // Pass original ID if your controller needs it
                                     updatedArtifact.getId(), updatedArtifact.getName(), updatedArtifact.getDescription(),
                                     updatedArtifact.getMaterial(), updatedArtifact.getDiscoveryDate(), updatedArtifact.getLocation());
        populateArtifactsList();
        clearInputFields();
        QMessageBox::information(this, "Success", "Artifact updated.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to update artifact: %1").arg(e.what()));
    }
}

void MainWindow::on_pushButton_Remove_clicked()
{
    if (!m_controller) return;
    QModelIndexList selectedIndexes = ui->artifactsListView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select an artifact to remove.");
        return;
    }
    // Fragile way to get ID, similar to update.
    QString selectedText = m_artifactsModel->data(selectedIndexes.first(), Qt::DisplayRole).toString();
    QString artifactId = selectedText.split(":").first().trimmed();

    if (artifactId.isEmpty()) {
        QMessageBox::warning(this, "Error", "Could not determine the ID of the selected artifact.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Remove", QString("Are you sure you want to remove artifact '%1'?").arg(artifactId),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            m_controller->removeArtifact(artifactId);
            populateArtifactsList();
            clearInputFields();
            QMessageBox::information(this, "Success", "Artifact removed.");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to remove artifact: %1").arg(e.what()));
        }
    }
}

void MainWindow::on_artifactsListView_clicked(const QModelIndex &index)
{
    if (!m_controller || !index.isValid()) return;

    // Fragile way to get ID, similar to update/remove.
    // A better approach would be to have the model store actual Artifact objects or at least their IDs
    // and retrieve the full object from the controller using the ID.
    QString selectedText = m_artifactsModel->data(index, Qt::DisplayRole).toString();
    QString artifactId = selectedText.split(":").first().trimmed();

    if (artifactId.isEmpty()) return;

    try {
        ArcheologicalArtifact artifact = m_controller->getArtifactById(artifactId);
        populateFieldsFromArtifact(artifact);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString("Could not retrieve artifact details: %1").arg(e.what()));
        clearInputFields(); // Clear fields if artifact not found or error
    }
}


void MainWindow::on_pushButton_Undo_clicked()
{
    if (!m_controller) return;
    
    if (m_controller->canUndo()) {
        m_controller->undo();
        populateArtifactsList();
        QMessageBox::information(this, "Undo", "Last action undone.");
    } else {
        QMessageBox::information(this, "Undo", "Nothing to undo.");
    }
}

void MainWindow::on_pushButton_Redo_clicked()
{
    if (!m_controller) return;
    
    if (m_controller->canRedo()) {
        m_controller->redo();
        populateArtifactsList();
        QMessageBox::information(this, "Redo", "Last undone action redone.");
    } else {
        QMessageBox::information(this, "Redo", "Nothing to redo.");
    }
}

void MainWindow::on_pushButton_ApplyFilter_clicked()
{
    if (!m_controller) return;
    
    QString filterText = ui->lineEdit_FilterCriteria->text().trimmed();
    
    if (filterText.isEmpty()) {
        // Show all artifacts if filter is empty
        populateArtifactsList();
        return;
    }
    
    try {
        // Apply a simple name-based filter (can be enhanced to support multiple criteria)
        std::vector<ArcheologicalArtifact> filteredArtifacts = 
            m_controller->filterArtifactsByName(filterText, false); // case-insensitive
        
        // Update the display with filtered results
        QStringList artifactDisplayList;
        for (const auto& artifact : filteredArtifacts) {
            artifactDisplayList.append(QString("%1: %2").arg(artifact.getId()).arg(artifact.getName()));
        }
        m_artifactsModel->setStringList(artifactDisplayList);
        
        QMessageBox::information(this, "Filter", 
            QString("Found %1 artifacts matching '%2'").arg(filteredArtifacts.size()).arg(filterText));
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Filter Error", QString("Failed to apply filter: %1").arg(e.what()));
    }
}
