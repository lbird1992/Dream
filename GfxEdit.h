//////////////////////////////////////////////////////////////////////////
//
// 编辑输入框类
//
// 作者：微妙的平衡(BOGY)
//
// 版本：For HGE v1.6
//
// 修改：李三哥 @ 2012-2-7
//////////////////////////////////////////////////////////////////////////
#ifndef GDIEDIT_H
#define GDIEDIT_H

#include "GfxFont.h"

class GfxEdit
{
public:

	///
	/// nEditWidth		编辑框宽度
	///	nFontColor		字体颜色
	/// lpFontName		字体名称
	///	nFontSize		字体大小
	/// bBold			是否粗体
	/// bItalic			是否斜体
	/// bAntialias		是否平滑
	///
	GfxEdit(UINT nEditWidth, DWORD nFontColor, CHAR* lpFontName = "宋体", UINT nFontSize = 12, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
	~GfxEdit(void);

	typedef enum tagDeletePos
	{
		DP_LEFT = 0,
		DP_RIGHT
	}TDP;

public:
	void			SetFocus(); // 设置编辑框焦点，只有拥有编辑框焦点才能响应输入
	void			KillFocus(); // 清除编辑框焦点，失去编辑框焦点后将不再响应输入
	bool			IsFocus();

	void			ClearCookie(); // 清除编辑框内容

	void			InsertCookie(const char* lpCookie); // 插入字符串到编辑框当前光标处
	const char*		GetCookie(); // 获取编辑框当前内容

	void			InsertCookieW(const wchar_t* lpCookie); // 插入字符串到编辑框当前光标处(宽字符模式)
	const wchar_t*	GetCookieW(); // 获取编辑框当前内容(宽字符模式)

	void			Updata();
	void			Render(float x, float y); //

private:
	void			OnKey( UINT nKey, UINT nRepCnt, UINT nFlags );
	void			OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	void			InsertChar(wchar_t aChar);
	void			DeleteChar(TDP aPos);

private:
	static const int COOKIE_SIZE = 1024;
	wchar_t			m_sCookie[COOKIE_SIZE];
	char			m_sBuffer[COOKIE_SIZE];

	FLOAT			m_nEditW;
	FLOAT			m_nEditH;

	UINT			m_nCharPos;		// 字符位置

	hgeRect			m_rcWorld;
    
	HGE*			m_pHGE;			// HGE指针
	hgeSprite*		m_pSprite;		// 光标精灵
	GfxFont*		m_pFont;		// 字体精灵

	static LRESULT CALLBACK GfxEditWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
};
#endif//GDIEDIT_H
