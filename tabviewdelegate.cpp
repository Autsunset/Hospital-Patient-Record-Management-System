#include "tabviewDelegate.h"
#include <QTextEdit>
#include <QDebug>
tabviewDelegate::tabviewDelegate(QObject *parent)
{}
QWidget *tabviewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{//设置单元格格式，编写下拉框的内容
    if(type == "sex"){
        QComboBox *editor = new QComboBox(parent);
        editor->addItem(tr("男"));
        editor->addItem(tr("女"));
        return editor;
    }
    else{//其他时候，长文本格式
        QTextEdit *editor = new QTextEdit(parent);
        return editor;
    }
}
void tabviewDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{//把表格的列和上个函数编写的类型进行对应。哪个类型是下拉框，哪个类型是长文本
    QString text =index.model()->data(index,Qt::DisplayRole).toString();
    if(type == "sex"||type == "status"||type == "warning"||type == "department"){
        QComboBox *cmb = static_cast<QComboBox*>(editor);
        cmb->setCurrentText(text);
    }
    else{
        QTextEdit *textedit = static_cast<QTextEdit*>(editor);
        textedit->setText(text);
    }
}
void tabviewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{//单元格获取从下拉框中选择的那个数据
    QString text;
    if(type == "sex"||type == "status"||type == "warning"||type == "department"){
        QComboBox *cmb = static_cast<QComboBox*>(editor);
        text= cmb->currentText();
    }
    else{
        QTextEdit *edit = static_cast<QTextEdit*>(editor);
        text= edit->toPlainText();
    }
    model->setData(index,text);
}
void tabviewDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


