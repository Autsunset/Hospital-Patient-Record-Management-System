#ifndef NEWCOMBBOXDELEGATE_H
#define NEWCOMBBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>
#include <QComboBox>

class newCombboxDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    newCombboxDelegate(QObject *parent=0);
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
private:
    QStringList m_type_list_;

};

#endif // NEWCOMBBOXDELEGATE_H

