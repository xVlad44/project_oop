#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel> // For QListView example
#include "../controller/artifact_controller.h" // Include your controller

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

private:
    Ui::MainWindow *ui;
    ArtifactController* m_controller; // Pointer to the controller
    QStringListModel* m_artifactsModel; // Example model for QListView

    void populateArtifactsList();
    void populateFieldsFromArtifact(const ArcheologicalArtifact& artifact);
    void clearInputFields();
    ArcheologicalArtifact getArtifactFromFields() const; // Helper to get data from input fields
};
#endif // MAINWINDOW_H
