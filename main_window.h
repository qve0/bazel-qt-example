#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QCalendarWidget>
#include <QLabel>
#include <QShortcut>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addTask();
    void deleteTask();
    void updateCalendar(const QDate &date);
    void updateDateEdit(const QDate &date);
    void validateInput(const QString &text);

private:
    QLineEdit *editTaskName;
    QDateEdit *dateDeadline;
    QComboBox *comboPriority;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    
    QListWidget *listTasks;
    QCalendarWidget *calendar;
    QLabel *statusLabel;
};

#endif