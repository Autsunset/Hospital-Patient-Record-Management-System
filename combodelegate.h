#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

#include <QItemDelegate>

class ComboDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    ComboDelegate() = delete;
    virtual ~ComboDelegate();
    ComboDelegate(const QStringList &items, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,  const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:
    void currentIndexChange(int row, int col) const;  //定义消息必须有const

private slots :

private:
    QStringList items;
};

#endif // COMBODELEGATE_H

