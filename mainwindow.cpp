#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow_2.h"
#include "combodelegate.h"
#include <QMainWindow>
#include <QTextCodec>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
       ui(new Ui::MainWindow)
   {
       ui->setupUi(this);
       db = QSqlDatabase::addDatabase("QMYSQL");

       db.setHostName("localhost");
       db.setDatabaseName("hospitalsql");
       db.setUserName("root");
       db.setPassword("12345678");
       if(!db.open()) {
          qDebug() << "error!\n";
       } else {
          qDebug() << "Open";
          model = new QSqlTableModel;
          model->setTable("patient");
          ui->tableView->setModel(model);
          QStringList sensorTypes;
          sensorTypes << ""
                      << "男"
                      << "女";

          ComboDelegate* delegateSensorTypes = new ComboDelegate(sensorTypes, this);
          ui->tableView->setItemDelegateForColumn(3, delegateSensorTypes);  // 第4列表格设置为下拉框形式

       }
   }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_search_clicked()
{
    // 清除QTableView的数据模型
    ui->tableView->setModel(nullptr);

    // 从界面获取查询条件
    int id = ui->lineEditid->text().toInt();
    QString name = ui->lineEditname->text();

    // 检查查询条件是否完整
    if(id == 0 || name.isNull() )
    {
        ui->textEdit->setText("请输入完整的信息");
        return;
    }

    // 创建QSqlTableModel对象
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("patient");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置查询条件
    QString filter = QString("id = %1 AND name = '%2'").arg(id).arg(name);
    model->setFilter(filter);

    // 从数据库中查询结果
    model->select();

    // 如果有结果则显示在QTableView中
    if(model->rowCount() > 0)
    {
        ui->tableView->setModel(model);
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked); // 设置编辑触发器为双击

    }
    else // 如果没有结果则提示用户
    {
        ui->textEdit->setText("没有这个病人");
    }
}


void MainWindow::on_pushButton_insert_clicked()
{
//在界面表格中添加一条空行
//当没有设置保存策略为手动提交时，在表格中直接输入数据后打回车即可新增一行
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);// 将保存策略设置为手动提交
    int rowNum = model->rowCount();
    model->insertRows(rowNum,1);
}


void MainWindow::on_checkaddButton_clicked()//对应按钮“确认插入”
{
    //设置保存策略为手动提交后，需要再添加这个按钮，进行下面的操作，才能将改变提交到数据库，否则，数据库不会做出任何改变
    int rowNum = model->rowCount();
    model->setData(model->index(rowNum,0),rowNum+1);
    if(model->submitAll()){
        model->database().commit();
        QMessageBox::information(nullptr,"添加成功","添加成功！");
    }
    else{
        model->database().rollback();
        QMessageBox::warning(this, tr("数据库错误"),
                             tr("数据库错误！"));
    }
}

void MainWindow::on_pushButton_delete_clicked()
{
    // 获取选定行的索引
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();

    if(!selectedIndexes.isEmpty())
    {
        // 获取选定行的模型
        QAbstractItemModel *model = ui->tableView->model();

        // 获取选定行的索引
        QModelIndex selectedIndex = selectedIndexes.at(0);
        int row = selectedIndex.row();

        // 删除选定行
        model->removeRow(row);
    }
}


void MainWindow::on_pushButton_searchall_clicked()
{
    // 清除QTableView的数据模型
    ui->tableView->setModel(nullptr);

    // 创建一个QSqlTableModel对象
    QSqlTableModel *model = new QSqlTableModel(this);

    // 设置模型的数据表
    model->setTable("patient");

    // 将模型的数据表绑定到QTableView中
    ui->tableView->setModel(model);

    // 选择整个数据表
    model->select();

    // 允许在QTableView中手动修改数据
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

}


void MainWindow::on_pushButton_modify_clicked()
{
    // 获取当前选定行的索引
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        ui->textEdit->setText("请先选中要修改的行");
        return;
    }
    int row = selectedIndexes.first().row();

    // 获取QSqlTableModel对象
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView->model());
    if(model)
    {
        // 获取要修改的单元格的列索引
        int column = ui->tableView->currentIndex().column();

        // 获取新的值
        QString newValue = ui->tableView->currentIndex().data().toString();

        // 将新值设置为模型中的单元格
        model->setData(model->index(row, column), newValue);

        // 提交所有未提交的更改到数据库中
        if(model->submitAll())
        {
            // 如果修改成功，弹出一个消息框
            QMessageBox::information(this, "修改成功", "数据已成功修改");
        }
        else
        {
            // 如果修改失败，显示一个错误消息
            QMessageBox::critical(this, "修改失败", "无法修改数据：\n" + model->lastError().text());
        }
    }
}

void MainWindow::on_checkaddButton_refresh_clicked()
{
    // 清除QTableView的数据模型
    ui->tableView->setModel(nullptr);
    model = new QSqlTableModel;
    model->setTable("patient");
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_prescribe_clicked() //打开mainwindow_2进行开方操作
{
    // 获取当前选定行的索引
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::information(this, "error", "请先选中要开方的患者");
        return;
    }
    int row = selectedIndexes.first().row();

    // 获取选定行的 patient ID
    QString patientId = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();

    // 创建 MainWindow_2 对象并传递 patient ID
    MainWindow_2 *w2 = new MainWindow_2(patientId);
    w2->show();
}




