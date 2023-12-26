#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RecvThread.h"
#include "packet_loss.h"
#include "packethread.h"
#include <QTableWidget>
#include "syswatcher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    RecvThread * t_Recvthread;
    packethread* t_packethread = new packethread;
    //board_mapping* board = new board_mapping;
    syswatcher* t_syswatcher = new syswatcher;

private slots:
    void on_action_triggered();

    void on_QuerenButton_clicked();

    void on_progressBar_valueChanged(int value);

    void file_analysis_over();

    void on_RealtimeButton_clicked();

private:
    QByteArray sd;
    Ui::MainWindow *ui;

    void NetTable_Init();
    void NetTable_Show();
    void SetTableValue_Int(QTableWidget * t_tableWidget, int row, int col, uint32_t Value);
    void SetTableValue_Float(QTableWidget * t_tableWidget, int row, int col, float Value);
    void SetTableValue_String(QTableWidget * t_tableWidget, int row, int col, QString Value);
    void LossPackTestOneLineShow(uint8_t pwlID, uint16_t * buf, uint16_t bufIndex, uint16_t bufLen,uint16_t raldata);
    void LossPackTable_Init();
    void LossPackTable_Show();
    uint8_t GetPwlIDIndex(uint8_t pwlID);
    void ClearTable(QTableWidget * t_tableWidget);
};

//extern PWlloss* Loss;

#endif // MAINWINDOW_H
