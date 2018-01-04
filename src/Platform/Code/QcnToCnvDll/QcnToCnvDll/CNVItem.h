#ifndef CNVITEM_H
#define CNVITEM_H

#define NV_BUFFER_LENGTH	(128*4)
#define NV_CONTENT_LENGTH	(128)
enum
{
	BRT,
	CNV,
	DU,
	QCN,
	RAWDATA
};
class CNVItemData
{
public:
	CNVItemData();
	~CNVItemData();	
	CNVItemData(const CNVItemData& rhs);
	CNVItemData& operator=(const CNVItemData& rhs);
	bool operator==(const CNVItemData& rhs) const;

	void	SetItemID(DWORD dwItemID);
	int	GetItemID() const;

	int		AddItemContent(BYTE* pbContent);
	BYTE *GetItemContent();

	int	GetNVContentLen() const;
	void	Reset();

	void SetNbOfElements(int NbOfEle);
	int GetNbOfElements() const;

	void SetIndex(int index);
	int GetIndex() const;

	short IsActive() const;
	void SetActive(short act);

	int   SetItemContent(BYTE* pbContent);
	void   SetNVContentLen(int len){ m_dwItemLength =len; };

private:

	DWORD	m_dwItemID;
	DWORD	m_dwItemLength;
	DWORD	m_dwBufferLength;
	BYTE*	m_pbItemContent;
	BYTE	m_pbItemLocalBuffer[NV_BUFFER_LENGTH];
	BYTE*	m_pbItemLargeBuffer;

	int		m_nNbOfElements;
	int		m_index;
	short	m_active; 
};
#endif