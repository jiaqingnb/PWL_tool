#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QFileDialog"
#include <QDirIterator>
#include <QFileInfo>
#include "QMenuBar"
#include "QFile"
#include "QTime"
#include "packet_loss.h"

PWlloss* Loss = new PWlloss;
uint8_t g_PwlNodeId[PWL_NODE_NUM] = {0,0,17, 18, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 245, 246, 243, 244, 241, 240, 255};
extern uint8_t NodeId[18];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->nettable->setWindowTitle("网络丢包测试");
    ui->frametable->setWindowTitle("滤包");
    /* 设置表格为20行 */
    ui->tableWidget->setRowCount(PWL_NODE_NUM);
    /* 清空进度条 */
    ui->progressBar->setValue(0);
    /* 表格信息初始化 */
    NetTable_Init();
    /* 丢包表格信息初始化 */
    LossPackTable_Init();
    /*丢包率节点初始化*/
    Loss->NodeInit();
    /* 隐藏行号 */
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->show();

    /* 创建接收线程 */
    t_Recvthread = new RecvThread(this);

    connect(t_Recvthread, SIGNAL(analysisOver()), this, SLOT(file_analysis_over()));
    connect(t_Recvthread, SIGNAL(showBarValue(int)), this, SLOT(on_progressBar_valueChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LossPackTable_Init()
{
    /* 设置行数 */
    ui->LossPack_Widget->setRowCount(ROW_MAX);
    /* 设置列数 */
    ui->LossPack_Widget->setColumnCount(PWL_NODE_NUM - 5);
    /* 隐藏列头标号 */
    ui->LossPack_Widget->verticalHeader()->setVisible(false);
    /* 设置禁止输入 */
    ui->LossPack_Widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /* 设置表头 */
    ui->LossPack_Widget->setHorizontalHeaderItem(12, new QTableWidgetItem("17"));
    ui->LossPack_Widget->setHorizontalHeaderItem(13, new QTableWidgetItem("18"));
    ui->LossPack_Widget->setHorizontalHeaderItem(14, new QTableWidgetItem("243"));
    ui->LossPack_Widget->setHorizontalHeaderItem(15, new QTableWidgetItem("244"));
    ui->LossPack_Widget->setHorizontalHeaderItem(16, new QTableWidgetItem("240"));
    ui->LossPack_Widget->setHorizontalHeaderItem(17, new QTableWidgetItem("241"));
    /* 单元格大小与内容匹配 */
    ui->LossPack_Widget->resizeColumnsToContents();
    ui->LossPack_Widget->resizeRowsToContents();
}

void MainWindow::LossPackTable_Show()
{
    for(int index = 0; index < PWL_NODE_NUM - 5; index++)
    {
        LossPackTestOneLineShow(Loss->board->boardsta[index].node, Loss->board->boardsta[index].data, Loss->board->boardsta[index].start_data, Loss->board->boardsta[index].end_data,Loss->board->boardsta[index].reaend_data);
    }
}

void MainWindow::on_action_triggered()
{
    QString path =  QFileDialog::getOpenFileName(this,"da","D:\\Work_wenjian");

    ui->lineEdit->setText(path);

    t_Recvthread->SetFilePath(path);

}

void MainWindow::on_QuerenButton_clicked()
{
    /* 表格信息初始化 */
    NetTable_Init();
    /* 开始接收线程 */
    ui->progressBar->setValue(0);
    t_Recvthread->start();
}

void MainWindow::on_progressBar_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

void MainWindow::file_analysis_over()
{
    ui->progressBar->setValue(100);
    //PWlloss::loss().setname(t_Recvthread->GetFilePath());
    Loss->setname(t_Recvthread->GetFilePath());
    qDebug()<<"soc = "<<packloss->soccnt;//调试打印——可删除
    qDebug()<<"soa = "<<packloss->soacnt;
    qDebug()<<"node[3].pres = "<<packloss->pwlnode[18].prescnt;
    /* 处理结束后 显示表格 */
    NetTable_Show();
    LossPackTable_Show();
    Loss->NodeInit();
    t_packethread->run();
}

void MainWindow::NetTable_Init()
{
    for(int i = 0; i < PWL_NODE_NUM; i++)
    {
        if(i == 0)
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem("SOC"));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem("0"));
        }
        else if (i == 1)
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem("SOA"));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem("0"));
        }
        else
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(g_PwlNodeId[i])));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem("0"));
        }
    }
}

void MainWindow::NetTable_Show()
{
    for(int i = 0; i < PWL_NODE_NUM; i++)
    {
        if(i==0)
        {
            SetTableValue_Int(ui->tableWidget, i,1,packloss->soccnt);
        }
        else if(i==1)
        {
            SetTableValue_Int(ui->tableWidget, i,1,packloss->soacnt);
        }
        else
        {
            /* 表格 显示 */
            SetTableValue_Int(ui->tableWidget, i,1,packloss->pwlnode[i].preqcnt);
            SetTableValue_Int(ui->tableWidget, i,2,packloss->pwlnode[i].prescnt);
            /*如果soc个数小于等于preq个数*/
            if(packloss->soccnt <= packloss->pwlnode[i].preqcnt || 0 == packloss->pwlnode[i].preqcnt)
            {
                SetTableValue_Int(ui->tableWidget, i,3,0);
            }
            else
            {
                SetTableValue_Int(ui->tableWidget, i,3,packloss->soccnt - packloss->pwlnode[i].preqcnt);
            }
            /*如果soc个数小于等于pres个数*/
            if(packloss->soccnt <= packloss->pwlnode[i].prescnt || 0 == packloss->pwlnode[i].prescnt)
            {
                SetTableValue_Int(ui->tableWidget, i,4,0);
            }
            else
            {
                SetTableValue_Int(ui->tableWidget, i, 4,packloss->soccnt - packloss->pwlnode[i].prescnt);
            }
        }


    }
}

void MainWindow::SetTableValue_Int(QTableWidget * t_tableWidget, int row, int col, uint32_t Value)
{
    t_tableWidget->setItem(row, col, new QTableWidgetItem(QString::number(Value)));
}

void MainWindow::SetTableValue_Float(QTableWidget * t_tableWidget, int row, int col, float Value)
{
    t_tableWidget->setItem(row, col, new QTableWidgetItem(QString::number(Value)));
}

void MainWindow::SetTableValue_String(QTableWidget * t_tableWidget, int row, int col, QString Value)
{
    t_tableWidget->setItem(row, col, new QTableWidgetItem(Value));
}

void MainWindow::LossPackTestOneLineShow(uint8_t pwlID, uint16_t * buf, uint16_t bufIndex, uint16_t bufLen,uint16_t raldata)
{
    uint8_t t_Col = 0u;
    uint16_t t_Row = 0u;
    t_Col = GetPwlIDIndex(pwlID);

    QColor a ;
    a = QColor(0x00, 0x00, 0xFF);

    if(pwlID == 17)
    {
        //uint8_t a = 0;
    }
    QString F1hexStr = QString::number(raldata, 16);
    ui->LossPack_Widget->setItem(t_Row, t_Col, new QTableWidgetItem(F1hexStr));

    ui->LossPack_Widget->item(t_Row, t_Col)->setBackgroundColor(QColor(255,0,0));

    t_Row++;
    for(int index = bufIndex; index < bufLen; index++)
    {
        if(buf[index] == 0)
        {
            QString hexStr = QString::number(index, 16);
            SetTableValue_String(ui->LossPack_Widget, t_Row++, t_Col, hexStr);
            //SetTableValue_Int(ui->LossPack_Widget, t_Row++, t_Col, index);
            qDebug()<<"index = "<< index;
        }
    }

    /* 单元格大小与内容匹配 */
    ui->LossPack_Widget->resizeColumnsToContents();
    ui->LossPack_Widget->resizeRowsToContents();
}

uint8_t MainWindow::GetPwlIDIndex(uint8_t pwlID)
{
    uint8_t retValue = 0;

    for(int index = 0; index < PWL_NODE_NUM - 5; index++)
    {
        if(NodeId[index] == pwlID)
        {
            retValue = index;
            break;
        }
    }

    return retValue;
}

void MainWindow::ClearTable(QTableWidget * t_tableWidget)
{
    t_tableWidget->clearContents();
}

