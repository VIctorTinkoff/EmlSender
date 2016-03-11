#ifndef RECIPIENTMODEL_H
#define RECIPIENTMODEL_H

#include <QAbstractListModel>
#include "Eml.h"

class RecipientModel : public QAbstractListModel
{
	Q_OBJECT

public:
	RecipientModel(QList<Eml::Recipient *> * recipients, QObject *parent = 0);
	~RecipientModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());




private:
	QList < Eml::Recipient *> * m_p_data;

	
};

#endif // RECIPIENTMODEL_H
