#include "newcombboxdelegate.h"
#include <QDebug>

newCombboxDelegate::newCombboxDelegate(QObject *parent)
{
    m_type_list_ << QStringLiteral("男性")
                    << QStringLiteral("女性");
}

QWidget *newCombboxDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
 {
     QComboBox *editor = new QComboBox(parent);
     editor->addItems(m_type_list_);//加入combobox的item列表
     return editor;
 }

 void newCombboxDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
 {
     QString text = index.model()->data(index, Qt::EditRole).toString();
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     int tindex = comboBox->findText(text);  //通过显示的文本内容得到当前item索引值
     comboBox->setCurrentIndex(tindex);
 }
 void newCombboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                         const QModelIndex &index) const
 {
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     QString text = comboBox->currentText(); //通过当前的索引得到文本内容
     model->setData(index, text, Qt::EditRole);

 }
 void newCombboxDelegate::updateEditorGeometry(QWidget *editor,
                                                 const QStyleOptionViewItem &option, const QModelIndex &index) const
 {
     editor->setGeometry(option.rect);//外观尺寸
 }

