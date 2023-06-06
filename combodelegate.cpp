#include "ComboDelegate.h"
#include <QtWidgets/QComboBox>
#include <QDebug>

ComboDelegate::ComboDelegate(const QStringList &items, QObject *parent) : QItemDelegate(parent)
{
    this->items = items;
}

ComboDelegate::~ComboDelegate()
{

}

QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(items);
    editor->setEditable(false);  // 不可编辑
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int currIndex = comboBox->findText(value);
    comboBox->setCurrentIndex(currIndex);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
    emit currentIndexChange(index.row(), index.column());   // 发送信号，实现其它联动更新
}

void ComboDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    (void)index;  // 防止编译警告
    editor->setGeometry(option.rect);
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->showPopup(); // 直接弹出下拉框，没有这一句双击后需要再点一次Combox才会弹出下拉框
}

