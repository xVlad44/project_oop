/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_Main;
    QListView *artifactsListView;
    QVBoxLayout *verticalLayout_Controls;
    QGroupBox *groupBox_Details;
    QFormLayout *formLayout_Details;
    QLabel *label_Id;
    QLineEdit *lineEdit_Id;
    QLabel *label_Name;
    QLineEdit *lineEdit_Name;
    QLabel *label_Description;
    QTextEdit *textEdit_Description;
    QLabel *label_Material;
    QLineEdit *lineEdit_Material;
    QLabel *label_DiscoveryDate;
    QDateEdit *dateEdit_DiscoveryDate;
    QLabel *label_Location;
    QLineEdit *lineEdit_Location;
    QHBoxLayout *horizontalLayout_Actions;
    QPushButton *pushButton_Add;
    QPushButton *pushButton_Update;
    QPushButton *pushButton_Remove;
    QGroupBox *groupBox_Filter;
    QHBoxLayout *horizontalLayout_Filter;
    QLineEdit *lineEdit_FilterCriteria;
    QPushButton *pushButton_ApplyFilter;
    QHBoxLayout *horizontalLayout_UndoRedo;
    QPushButton *pushButton_Undo;
    QPushButton *pushButton_Redo;
    QSpacerItem *verticalSpacer_Controls;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_Main = new QHBoxLayout(centralwidget);
        horizontalLayout_Main->setObjectName("horizontalLayout_Main");
        artifactsListView = new QListView(centralwidget);
        artifactsListView->setObjectName("artifactsListView");

        horizontalLayout_Main->addWidget(artifactsListView);

        verticalLayout_Controls = new QVBoxLayout();
        verticalLayout_Controls->setObjectName("verticalLayout_Controls");
        groupBox_Details = new QGroupBox(centralwidget);
        groupBox_Details->setObjectName("groupBox_Details");
        formLayout_Details = new QFormLayout(groupBox_Details);
        formLayout_Details->setObjectName("formLayout_Details");
        label_Id = new QLabel(groupBox_Details);
        label_Id->setObjectName("label_Id");

        formLayout_Details->setWidget(0, QFormLayout::ItemRole::LabelRole, label_Id);

        lineEdit_Id = new QLineEdit(groupBox_Details);
        lineEdit_Id->setObjectName("lineEdit_Id");

        formLayout_Details->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit_Id);

        label_Name = new QLabel(groupBox_Details);
        label_Name->setObjectName("label_Name");

        formLayout_Details->setWidget(1, QFormLayout::ItemRole::LabelRole, label_Name);

        lineEdit_Name = new QLineEdit(groupBox_Details);
        lineEdit_Name->setObjectName("lineEdit_Name");

        formLayout_Details->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_Name);

        label_Description = new QLabel(groupBox_Details);
        label_Description->setObjectName("label_Description");

        formLayout_Details->setWidget(2, QFormLayout::ItemRole::LabelRole, label_Description);

        textEdit_Description = new QTextEdit(groupBox_Details);
        textEdit_Description->setObjectName("textEdit_Description");

        formLayout_Details->setWidget(2, QFormLayout::ItemRole::FieldRole, textEdit_Description);

        label_Material = new QLabel(groupBox_Details);
        label_Material->setObjectName("label_Material");

        formLayout_Details->setWidget(3, QFormLayout::ItemRole::LabelRole, label_Material);

        lineEdit_Material = new QLineEdit(groupBox_Details);
        lineEdit_Material->setObjectName("lineEdit_Material");

        formLayout_Details->setWidget(3, QFormLayout::ItemRole::FieldRole, lineEdit_Material);

        label_DiscoveryDate = new QLabel(groupBox_Details);
        label_DiscoveryDate->setObjectName("label_DiscoveryDate");

        formLayout_Details->setWidget(4, QFormLayout::ItemRole::LabelRole, label_DiscoveryDate);

        dateEdit_DiscoveryDate = new QDateEdit(groupBox_Details);
        dateEdit_DiscoveryDate->setObjectName("dateEdit_DiscoveryDate");
        dateEdit_DiscoveryDate->setCalendarPopup(true);

        formLayout_Details->setWidget(4, QFormLayout::ItemRole::FieldRole, dateEdit_DiscoveryDate);

        label_Location = new QLabel(groupBox_Details);
        label_Location->setObjectName("label_Location");

        formLayout_Details->setWidget(5, QFormLayout::ItemRole::LabelRole, label_Location);

        lineEdit_Location = new QLineEdit(groupBox_Details);
        lineEdit_Location->setObjectName("lineEdit_Location");

        formLayout_Details->setWidget(5, QFormLayout::ItemRole::FieldRole, lineEdit_Location);


        verticalLayout_Controls->addWidget(groupBox_Details);

        horizontalLayout_Actions = new QHBoxLayout();
        horizontalLayout_Actions->setObjectName("horizontalLayout_Actions");
        pushButton_Add = new QPushButton(centralwidget);
        pushButton_Add->setObjectName("pushButton_Add");

        horizontalLayout_Actions->addWidget(pushButton_Add);

        pushButton_Update = new QPushButton(centralwidget);
        pushButton_Update->setObjectName("pushButton_Update");

        horizontalLayout_Actions->addWidget(pushButton_Update);

        pushButton_Remove = new QPushButton(centralwidget);
        pushButton_Remove->setObjectName("pushButton_Remove");

        horizontalLayout_Actions->addWidget(pushButton_Remove);


        verticalLayout_Controls->addLayout(horizontalLayout_Actions);

        groupBox_Filter = new QGroupBox(centralwidget);
        groupBox_Filter->setObjectName("groupBox_Filter");
        horizontalLayout_Filter = new QHBoxLayout(groupBox_Filter);
        horizontalLayout_Filter->setObjectName("horizontalLayout_Filter");
        lineEdit_FilterCriteria = new QLineEdit(groupBox_Filter);
        lineEdit_FilterCriteria->setObjectName("lineEdit_FilterCriteria");

        horizontalLayout_Filter->addWidget(lineEdit_FilterCriteria);

        pushButton_ApplyFilter = new QPushButton(groupBox_Filter);
        pushButton_ApplyFilter->setObjectName("pushButton_ApplyFilter");

        horizontalLayout_Filter->addWidget(pushButton_ApplyFilter);


        verticalLayout_Controls->addWidget(groupBox_Filter);

        horizontalLayout_UndoRedo = new QHBoxLayout();
        horizontalLayout_UndoRedo->setObjectName("horizontalLayout_UndoRedo");
        pushButton_Undo = new QPushButton(centralwidget);
        pushButton_Undo->setObjectName("pushButton_Undo");

        horizontalLayout_UndoRedo->addWidget(pushButton_Undo);

        pushButton_Redo = new QPushButton(centralwidget);
        pushButton_Redo->setObjectName("pushButton_Redo");

        horizontalLayout_UndoRedo->addWidget(pushButton_Redo);


        verticalLayout_Controls->addLayout(horizontalLayout_UndoRedo);

        verticalSpacer_Controls = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Controls->addItem(verticalSpacer_Controls);


        horizontalLayout_Main->addLayout(verticalLayout_Controls);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Archeological Artifacts Inventory", nullptr));
        groupBox_Details->setTitle(QCoreApplication::translate("MainWindow", "Artifact Details", nullptr));
        label_Id->setText(QCoreApplication::translate("MainWindow", "ID:", nullptr));
        label_Name->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        label_Description->setText(QCoreApplication::translate("MainWindow", "Description:", nullptr));
        label_Material->setText(QCoreApplication::translate("MainWindow", "Material:", nullptr));
        label_DiscoveryDate->setText(QCoreApplication::translate("MainWindow", "Discovery Date:", nullptr));
        label_Location->setText(QCoreApplication::translate("MainWindow", "Location:", nullptr));
        pushButton_Add->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        pushButton_Update->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        pushButton_Remove->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        groupBox_Filter->setTitle(QCoreApplication::translate("MainWindow", "Filter", nullptr));
        pushButton_ApplyFilter->setText(QCoreApplication::translate("MainWindow", "Apply Filter", nullptr));
        pushButton_Undo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        pushButton_Redo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
