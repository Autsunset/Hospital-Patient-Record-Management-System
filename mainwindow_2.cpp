#include "mainwindow_2.h"
#include "mainwindow.h"
#include "ui_mainwindow_2.h"
#include "combodelegate.h"
#include <QMainWindow>
#include <QTextCodec>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>



MainWindow_2::MainWindow_2(QString patientId, QWidget *parent)
//(QWidget *parent)
    :QMainWindow(parent),
    ui(new Ui::MainWindow_2)
{
    ui->setupUi(this);
    // 查询 patient 表以获取对应的数据
    QSqlQuery query;
    query.prepare("SELECT * FROM patient WHERE id = :id");
    query.bindValue(":id", patientId);
    if(!query.exec() || !query.next())
    {
        QMessageBox::critical(this, "Error", "无法获取选定行的数据: " + query.lastError().text());
        return;
    }
    QString name = query.value("name").toString();
    QString age = query.value("age").toString();
    QString sex = query.value("sex").toString();
    QString tel = query.value("tel").toString();
    QString address = query.value("address").toString();

    // 初始化模型并设置数据
    model = new QSqlTableModel(this);
    model->setTable("patient");
    model->setFilter(QString("id = '%1'").arg(patientId));
    model->select();

    // 将数据设置为 tableView 的数据
    model->setData(model->index(0, 1), name);
    model->setData(model->index(0, 2), age);
    model->setData(model->index(0, 3), sex);
    model->setData(model->index(0, 4), tel);
    model->setData(model->index(0, 5), address);
    ui->tableView->setModel(model);
}

MainWindow_2::~MainWindow_2()
{
    delete ui;
}



void MainWindow_2::on_pushButton_searchallpatient_clicked()
{
    // 清除QTableView的数据模型
    ui->tableView->setModel(nullptr);

    // 创建一个QSqlTableModel对象
    QSqlTableModel *model = new QSqlTableModel(this);

    // 设置模型的数据表
    model->setTable("patient");

    // 将模型的数据表绑定到QTableView中
    ui->tableView->setModel(model);


    QStringList sensorTypes;
    sensorTypes << ""
                << "男"
                << "女";

    ComboDelegate* delegateSensorTypes = new ComboDelegate(sensorTypes, this);
    ui->tableView->setItemDelegateForColumn(3, delegateSensorTypes);  // 第4列表格设置为下拉框形式

    // 选择整个数据表
    model->select();



    // 允许在QTableView中手动修改数据
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

}

void MainWindow_2::on_pushButton_search_medical_records_clicked()
{
    // 获取当前选定行的索引
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::information(this, "error", "请先选中要查找病历的病人");
        return;
    }

    // 获取选中行的主键ID
    QString patient_id = ui->tableView->model()->data(ui->tableView->model()->index(selectedIndexes.at(0).row(), 0)).toString();

    // 查询病历记录
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("case_history");
    model->setFilter(QString("id = %1").arg(patient_id));
    model->select();
    ui->tableView_2->setModel(model);
}

void MainWindow_2::on_pushButton_delete_clicked()
{
    // 获取当前选定行的模型索引
    QModelIndexList selectedIndexes = ui->tableView_2->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::information(this, "error", "请先选中要删除的病历记录");
        return;
    }

    // 获取选中行的主键 Number
    QString number = ui->tableView_2->model()->data(ui->tableView_2->model()->index(selectedIndexes.at(0).row(), 0)).toString();

    // 删除选中行
    QSqlQuery query;
    query.prepare("DELETE FROM case_history WHERE number = :number");
    query.bindValue(":number", number);

    if(query.exec())
    {
        QMessageBox::information(this, "Success", "病历记录删除成功！");
        // 更新QTableView中的数据模型
        QSqlTableModel *model = new QSqlTableModel(this);
        model->setTable("case_history");
        model->setFilter(QString("number = %1").arg(number));
        model->select();
        ui->tableView_2->setModel(model);
    }
    else
    {
        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::information(this, "Error", "病历记录删除失败！");
    }
}


void MainWindow_2::on_pushButton_create_medical_records_clicked()
{
    // 获取当前选定行的索引
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::information(this, "error", "请先选中要开病历的病人");
        return;
    }

    // 获取选中行的主键ID
    QString patient_id = ui->tableView->model()->data(ui->tableView->model()->index(selectedIndexes.at(0).row(), 0)).toString();

    // 在case_history表中插入新记录
    QSqlQuery query;
    query.prepare("INSERT INTO case_history (id, doctorname, number, disease, prescription) "
                  "VALUES (:id, :doctorname, :number, :disease, :prescription)");
    query.bindValue(":id", patient_id);
    query.bindValue(":doctorname", "");
    query.bindValue(":number", QString::number(query.lastInsertId().toInt()));
    query.bindValue(":disease", "");
    query.bindValue(":prescription", "");

    if(query.exec())
    {
        QMessageBox::information(this, "Success", "病历创建成功！");

        // 更新QTableView中的数据模型
        QSqlTableModel *model = new QSqlTableModel(this);
        model->setTable("case_history");
        model->setFilter(QString("id = %1").arg(patient_id));

        if(model->select())
        {
            // 在 tableView_2 中显示新插入的病历记录
            ui->tableView_2->setModel(model);
        }
        else
        {
            qDebug() << "Error selecting records: " << model->lastError().text();
            QMessageBox::information(this, "Error", "病历创建成功，但无法更新病历记录！");
        }
    }
    else
    {
        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::information(this, "Error", "病历创建失败！");
    }

}

void MainWindow_2::on_pushButton_confirm_prescription_clicked()
{
    // 获取当前选定行的索引
    QModelIndexList selectedIndexes = ui->tableView_2->selectionModel()->selectedIndexes();
    if(selectedIndexes.isEmpty())
    {
        QMessageBox::information(this, "error", "请先选中要修改的行");
        return;
    }
    int row = selectedIndexes.first().row();

    // 获取QSqlTableModel对象
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView_2->model());
    if(model)
    {
        // 获取要修改的单元格的列索引
        int column = ui->tableView_2->currentIndex().column();

        // 获取新的值
        QString newValue = ui->tableView_2->currentIndex().data().toString();

        // 将新值设置为模型中的单元格
        model->setData(model->index(row, column), newValue);

        // 提交所有未提交的更改到数据库中
        if(model->submitAll())
        {
            // 如果修改成功，弹出一个消息框
            QSqlDatabase::database().commit(); // 提交事务
            QMessageBox::information(this, "修改成功", "数据已成功修改");
        }
        else
        {
            // 如果修改失败，显示一个错误消息
            QSqlDatabase::database().rollback(); // 回滚事务
            QMessageBox::critical(this, "修改失败", "无法修改数据：\n" + model->lastError().text());
        }
    }
}

void MainWindow_2::on_pushButton_searchall_medical_records_clicked()
{
    // 初始化模型并设置数据
    model = new QSqlTableModel(this);
    model->setTable("case_history");
    model->select();

    // 将模型设置为 tableView 的模型
    ui->tableView_2->setModel(model);
}
