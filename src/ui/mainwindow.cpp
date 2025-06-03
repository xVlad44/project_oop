#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> // For simple feedback
#include <QDebug>      // For debugging output

MainWindow::MainWindow(ArtifactController* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller)
    , m_artifactsModel(new QStringListModel(this))
    , m_filterTypeComboBox(nullptr)
    , m_startDateEdit(nullptr)
    , m_endDateEdit(nullptr)
    , m_activeFilters(new QListWidget(this))
{
    ui->setupUi(this);
    
    // Setup the existing UI components
    if (!m_controller) {
        QMessageBox::critical(this, "Error", "Controller not available. Application might not function correctly.");
        return;
    }
    
    ui->artifactsListView->setModel(m_artifactsModel);
    
    // Set up the filter UI components
    setupFilterUI();
    
    // Add the active filters list below the filter controls
    ui->verticalLayout_Controls->addWidget(new QLabel("Active Filters:", this));
    ui->verticalLayout_Controls->addWidget(m_activeFilters);
    
    // Connect the remove filter button
    QPushButton* removeFilterButton = new QPushButton("Remove Selected Filter", this);
    connect(removeFilterButton, &QPushButton::clicked, this, &MainWindow::onRemoveFilterClicked);
    ui->verticalLayout_Controls->addWidget(removeFilterButton);
    
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
    
    // Add special "New Artifact" item at the top
    artifactDisplayList.append("+ Create New Artifact");
    
    // Add a visual separator
    artifactDisplayList.append("-------------------");
    
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
    
    int row = selectedIndexes.first().row();
    
    // Check if the "New Artifact" item or separator was selected
    if (row == 0 || row == 1) {
        QMessageBox::warning(this, "Selection Error", "Please select an actual artifact to update.");
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
    
    int row = selectedIndexes.first().row();
    
    // Check if the "New Artifact" item or separator was selected
    if (row == 0 || row == 1) {
        QMessageBox::warning(this, "Selection Error", "Please select an actual artifact to remove.");
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

    int row = index.row();
    QString selectedText = m_artifactsModel->data(index, Qt::DisplayRole).toString();
    
    // Check if "New Artifact" item was clicked (first row)
    if (row == 0 && selectedText == "+ Create New Artifact") {
        clearInputFields();
        // Set focus to first input field to start entering data
        ui->lineEdit_Id->setFocus();
        return;
    }
    
    // Check if separator was clicked (second row)
    if (row == 1 && selectedText == "-------------------") {
        return; // Do nothing for separator click
    }

    // Original code for handling artifact selection
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
    
    QString filterType = m_filterTypeComboBox->currentData().toString();
    QString filterText = ui->lineEdit_FilterCriteria->text().trimmed();
    
    if (filterType == "date") {
        // Create date range filter
        QDate startDate = m_startDateEdit->date();
        QDate endDate = m_endDateEdit->date();
        
        if (startDate > endDate) {
            QMessageBox::warning(this, "Invalid Date Range", 
                "Start date must be before or equal to end date.");
            return;
        }
        
        auto filter = std::make_unique<DateRangeFilter>(startDate, endDate);
        addActiveFilter(std::move(filter), 
            QString("Date: %1 to %2").arg(startDate.toString("yyyy-MM-dd"))
                                    .arg(endDate.toString("yyyy-MM-dd")));
    }
    else if (!filterText.isEmpty()) {
        std::unique_ptr<FilterStrategy> filter;
        QString displayText;
        
        // Create the appropriate filter based on type
        if (filterType == "name") {
            filter = std::make_unique<NameFilter>(filterText, false);
            displayText = QString("Name: contains '%1'").arg(filterText);
        }
        else if (filterType == "id") {
            filter = std::make_unique<IdFilter>(filterText, false);
            displayText = QString("ID: contains '%1'").arg(filterText);
        }
        else if (filterType == "material") {
            filter = std::make_unique<MaterialFilter>(filterText, false);
            displayText = QString("Material: contains '%1'").arg(filterText);
        }
        else if (filterType == "location") {
            filter = std::make_unique<LocationFilter>(filterText, false);
            displayText = QString("Location: contains '%1'").arg(filterText);
        }
        
        if (filter) {
            addActiveFilter(std::move(filter), displayText);
        }
    }
}

void MainWindow::setupFilterUI() {
    // Create the filter combo box if it doesn't exist
    if (!m_filterTypeComboBox) {
        m_filterTypeComboBox = new QComboBox(ui->groupBox_Filter);
        m_filterTypeComboBox->addItem("Name", "name");
        m_filterTypeComboBox->addItem("ID", "id");
        m_filterTypeComboBox->addItem("Material", "material");
        m_filterTypeComboBox->addItem("Location", "location");
        m_filterTypeComboBox->addItem("Date Range", "date");
        ui->horizontalLayout_Filter->insertWidget(0, m_filterTypeComboBox);
    }

    // Create date range widgets if they don't exist
    if (!m_startDateEdit) {
        m_startDateEdit = new QDateEdit(ui->groupBox_Filter);
        m_startDateEdit->setCalendarPopup(true);
        m_startDateEdit->setDate(QDate::currentDate().addYears(-100));
        m_startDateEdit->setVisible(false);
        ui->horizontalLayout_Filter->insertWidget(2, m_startDateEdit);
    }

    if (!m_endDateEdit) {
        m_endDateEdit = new QDateEdit(ui->groupBox_Filter);
        m_endDateEdit->setCalendarPopup(true);
        m_endDateEdit->setDate(QDate::currentDate());
        m_endDateEdit->setVisible(false);
        ui->horizontalLayout_Filter->insertWidget(3, m_endDateEdit);
    }

    // Connect signals
    connect(m_filterTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
        this, &MainWindow::onFilterTypeChanged);
    
    // Add reset filters button
    QPushButton* resetButton = new QPushButton("Reset Filters", ui->groupBox_Filter);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetFilters);
    ui->horizontalLayout_Filter->addWidget(resetButton);
}

void MainWindow::resetFilters() {
    m_activeFilters->clear();
    m_compositeFilter.reset();
    populateArtifactsList();
}

void MainWindow::onFilterTypeChanged(int index) {
    QString type = m_filterTypeComboBox->currentData().toString();
    bool isDateRange = (type == "date");
    
    ui->lineEdit_FilterCriteria->setVisible(!isDateRange);
    m_startDateEdit->setVisible(isDateRange);
    m_endDateEdit->setVisible(isDateRange);
}

void MainWindow::addActiveFilter(std::unique_ptr<FilterStrategy> filter, const QString& displayText) {
    if (!filter) return;
    
    if (!m_compositeFilter) {
        m_compositeFilter = std::make_unique<AndFilter>();
    }
    
    // Add to the composite filter
    m_compositeFilter->addFilter(filter->clone());
    
    // Add to the UI list with filter type as user data
    QListWidgetItem* item = new QListWidgetItem(displayText);
    
    // Store filter type and parameters for potential rebuilding
    QString filterType = m_filterTypeComboBox->currentData().toString();
    QVariantMap filterData;
    filterData["type"] = filterType;
    
    if (filterType == "date") {
        filterData["startDate"] = m_startDateEdit->date();
        filterData["endDate"] = m_endDateEdit->date();
    } else {
        filterData["text"] = ui->lineEdit_FilterCriteria->text().trimmed();
    }
    
    item->setData(Qt::UserRole, QVariant::fromValue(filterData));
    m_activeFilters->addItem(item);
    
    // Apply the filter
    applyCurrentFilter();
}

void MainWindow::applyCurrentFilter() {
    if (!m_controller || !m_compositeFilter) {
        populateArtifactsList();  // Show all if no filter
        return;
    }
    
    QStringList displayList;
    
    // Always keep the "New Artifact" option at the top
    displayList.append("+ Create New Artifact");
    displayList.append("-------------------");
    
    ArtifactFilter filter(m_compositeFilter->clone());
    std::vector<ArcheologicalArtifact> artifacts = m_controller->getAllArtifacts();
    auto filteredArtifacts = filter.filter(artifacts);
    
    // Update display with filtered artifacts
    for (const auto& artifact : filteredArtifacts) {
        displayList.append(QString("%1: %2").arg(artifact.getId()).arg(artifact.getName()));
    }
    m_artifactsModel->setStringList(displayList);
}

void MainWindow::onRemoveFilterClicked() {
    int currentRow = m_activeFilters->currentRow();
    if (currentRow >= 0) {
        // Remove the item from UI
        m_activeFilters->takeItem(currentRow);
        
        // Rebuild the composite filter from scratch using remaining UI items
        rebuildCompositeFilterFromUI();
        
        // Apply the updated filter
        applyCurrentFilter();
    }
}

// Add this new helper method to rebuild filters from UI items
void MainWindow::rebuildCompositeFilterFromUI() {
    // Reset the composite filter
    m_compositeFilter.reset();
    
    // If no filters remain, just return
    if (m_activeFilters->count() == 0) {
        return;
    }
    
    // Create a new composite filter
    m_compositeFilter = std::make_unique<AndFilter>();
    
    // Loop through all items in the active filters list
    for (int i = 0; i < m_activeFilters->count(); ++i) {
        QListWidgetItem* item = m_activeFilters->item(i);
        QVariantMap filterData = item->data(Qt::UserRole).toMap();
        
        QString filterType = filterData["type"].toString();
        std::unique_ptr<FilterStrategy> filter;
        
        // Recreate the appropriate filter based on stored type and parameters
        if (filterType == "date") {
            QDate startDate = filterData["startDate"].toDate();
            QDate endDate = filterData["endDate"].toDate();
            filter = std::make_unique<DateRangeFilter>(startDate, endDate);
        } 
        else {
            QString text = filterData["text"].toString();
            
            if (filterType == "name") {
                filter = std::make_unique<NameFilter>(text, false);
            }
            else if (filterType == "id") {
                filter = std::make_unique<IdFilter>(text, false);
            }
            else if (filterType == "material") {
                filter = std::make_unique<MaterialFilter>(text, false);
            }
            else if (filterType == "location") {
                filter = std::make_unique<LocationFilter>(text, false);
            }
        }
        
        // Add to composite filter if successfully created
        if (filter) {
            m_compositeFilter->addFilter(filter->clone());
        }
    }
}
