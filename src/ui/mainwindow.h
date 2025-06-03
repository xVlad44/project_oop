#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel> // For QListView example
#include "../controller/artifact_controller.h" // Include your controller
#include <QComboBox>
#include <QDateEdit>
#include <QListWidget>
#include <memory> // For std::unique_ptr


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Pass the controller to the MainWindow
    MainWindow(ArtifactController* controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Add_clicked();
    void on_pushButton_Update_clicked();
    void on_pushButton_Remove_clicked();
    void on_pushButton_Undo_clicked();
    void on_pushButton_Redo_clicked();
    void on_pushButton_ApplyFilter_clicked();
    void on_artifactsListView_clicked(const QModelIndex &index); // To populate fields when an item is clicked
    void onFilterTypeChanged(int index);
    void onRemoveFilterClicked();
    void resetFilters();

private:
    Ui::MainWindow *ui;
    ArtifactController* m_controller; // Pointer to the controller
    QStringListModel* m_artifactsModel; // Example model for QListView
    
    // Filter UI components
    QComboBox* m_filterTypeComboBox;
    QDateEdit* m_startDateEdit;
    QDateEdit* m_endDateEdit;
    QListWidget* m_activeFilters;
    std::unique_ptr<AndFilter> m_compositeFilter;
    
    void setupFilterUI();
    void populateArtifactsList();
    void populateFieldsFromArtifact(const ArcheologicalArtifact& artifact);
    void clearInputFields();
    ArcheologicalArtifact getArtifactFromFields() const; // Helper to get data from input fields
    void addActiveFilter(std::unique_ptr<FilterStrategy> filter, const QString& displayText);
    void applyCurrentFilter();
    void rebuildCompositeFilterFromUI(); // Add this helper method
};
#endif // MAINWINDOW_H
