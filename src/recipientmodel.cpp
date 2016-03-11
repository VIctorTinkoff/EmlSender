#include "recipientmodel.h"

RecipientModel::RecipientModel(QList<Eml::Recipient *> * recipients, QObject *parent /*= 0*/)
: QAbstractListModel(parent)
, m_p_data(recipients)
{

}

RecipientModel::~RecipientModel()
{

}

int RecipientModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return m_p_data->count();

}

QVariant RecipientModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_p_data->size())
		return QVariant();

	if (role == Qt::DisplayRole || role == Qt::EditRole)
		return m_p_data->at(index.row())->title;
	else
		return QVariant();

}

QVariant RecipientModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal)
		return QString("Column %1").arg(section);
	else
        return QString("Row %1").arg(section);
}

Qt::ItemFlags RecipientModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool RecipientModel::setData(const QModelIndex & index, const QVariant &value, int role /*= Qt::DisplayRole*/)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		m_p_data->at(index.row())->title = value.toString();
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

bool RecipientModel::removeRows(int position, int rows, const QModelIndex &parent /*= QModelIndex()*/)
{
	if (position < 0)
		return false;

	beginRemoveRows(QModelIndex(),position, position + rows - 1);

	for (int row = 0;  row < rows; ++row)
	{
		m_p_data->removeAt(position);
	}

	endRemoveRows();
	return true;

}

bool RecipientModel::insertRows(int position, int rows, const QModelIndex &index /*= QModelIndex()*/)
{
	if (position < 0)
		return false;

	beginInsertRows(QModelIndex(), position, position + rows - 1);
	
	for (int row = 0; row < rows; ++row)
	{
		m_p_data->insert(position, new Eml::Recipient);
	}
	endInsertRows();
	return true;
	
}

