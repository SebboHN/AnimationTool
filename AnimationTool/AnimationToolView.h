
// AnimationToolView.h : interface of the CAnimationToolView class
//

#pragma once


class CAnimationToolView : public CView
{
protected: // create from serialization only
	CAnimationToolView() noexcept;
	DECLARE_DYNCREATE(CAnimationToolView)

// Attributes
public:
	CAnimationToolDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CAnimationToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AnimationToolView.cpp
inline CAnimationToolDoc* CAnimationToolView::GetDocument() const
   { return reinterpret_cast<CAnimationToolDoc*>(m_pDocument); }
#endif

