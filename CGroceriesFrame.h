
// MainFrm.h : interface of the CGroceriesFrame class
//

#pragma once

class GroceriesCatalog;
class StationeryDockBar;
class GroceriesBasicPage;
class StuffDetailDockBar;

class CGroceriesFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CGroceriesFrame();
	DECLARE_DYNCREATE(CGroceriesFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CGroceriesFrame();

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

protected:
	StationeryDockBar* m_pStationeryDockBar;
	GroceriesBasicPage* m_pBasicPage;
	StuffDetailDockBar* m_pStuffDetailDockBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowStioneryDockBar();
	afx_msg void OnUpdatetioneryDockBar(CCmdUI* pUI);

	afx_msg void OnShowPage();
	afx_msg void OnUpdateOnShowPage(CCmdUI* pUI);

	afx_msg void OnShowStuffDetails();
	afx_msg void OnUpdateStuffDetails(CCmdUI* pUI);
	

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRefresh();

	// Get docking window
	GroceriesBasicPage* GetPage();
	StationeryDockBar* GetStationeryDockBar();
	StuffDetailDockBar* GetStuffDetailDockBar();

	// Get root
	GroceriesCatalog* GetRoot();
};


