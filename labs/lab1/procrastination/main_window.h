#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QListWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QProgressBar>
#include <vector>

//статусы билета
enum TicketStatus {
    StatusDefault = 0, 
    StatusYellow,      
    StatusGreen        
};

//хранение данных о билете
struct Ticket {
    QString name;
    TicketStatus status;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onCountChanged(int count);
    void onTicketClicked(QListWidgetItem* item);
    void onTicketDoubleClicked(QListWidgetItem* item);
    void onNameReturnPressed();
    void onStatusChanged(int index);
    void onNextClicked();
    void onPrevClicked();

private:
    void setupUi();
    void updateProgressBars();
    void selectTicket(int index);
    void updateTicketViewInList(int index);
    void updateButtonsState();

    //билеты
    std::vector<Ticket> m_tickets;
    int m_currentIndex;
    std::vector<int> m_history;
    bool m_isUpdatingUI;

    //UI элементы
    QSpinBox* spinCount;
    QListWidget* listTickets;
    
    QGroupBox* groupQuestion;
    QLabel* lblNumber;
    QLabel* lblName;
    QLineEdit* editName;
    QComboBox* comboStatus;
    QPushButton* btnNext;
    QPushButton* btnPrev;
    
    QProgressBar* progTotal;
    QProgressBar* progGreen;
};

#endif