#ifndef _SCORE_RECORD_LAYER_H
#define _SCORE_RECORD_LAYER_H

#define TITLE_COLOR ccc3(228, 255, 0)
#define NAME_COLOR ccc3(42, 173, 255)
#define BEFORE_3 3
#define BEFORE_30 30

class CScoreRecordLayer : public CGameLayer ,public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
public:
	CScoreRecordLayer();
	virtual ~CScoreRecordLayer();
	virtual void OnInit();

	virtual void OnLayerIn(float fDuration);
	virtual void OnLayerOut(float fDuration);
	virtual void OnLayerIn(CGameLayer* pOutLayer, ELayerAni enAniType, float fDuration = 0.5f);
	virtual void OnLayerOut(CGameLayer* pInLayer, ELayerAni enAniType, float fDuration = 0.5f);

	virtual void onHttpRequestCompleted(cocos2d::CCNode *sender, void *data);

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

public:
	void touchCancel(CCObject *sender, CCControlEvent controlEvent);
	void Hide();

protected:
	void RequestWeb();
	void ParseScoreRecordXML(TiXmlDocument* xmldoc);
	void ParseScoreRecordXML(TiXmlElement* pEnumElement, bool bAllNext);

private:
	CCSprite *m_pBk;
	CCSprite *m_pLoading;
	CCTableView * m_ptableView;
	const char* m_xmlfilepath;

	std::vector<tagLocalScoreRecord> m_vScoreRecord;
};
#endif