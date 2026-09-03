#ifdef __cplusplus
 extern "C" {
#endif
    
#ifndef WIN_DRAW_STYLE
    #define WIN_DRAW_STYLE

void OuterPanel_Draw(Windows *win, Graphics *Gr, int sx, int sy, int wid, int heigh);	 
void InnerPanel_Draw(Windows *win, Graphics *Gr, int sx, int sy, int wid, int heigh);	 
void TextBoxStyle_Draw(Windows *win, Graphics *Gr, int sx, int sy, int width, int height);	
#ifdef __cplusplus
}
#endif

#endif // WIN_DRAW_STYLE	

