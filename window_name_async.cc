void write_window_name (FILE *& out)
{
  Display *disp = XOpenDisplay(NULL);
  Window win;
  XEvent xevent;

  int revert;
  Atom type;
  int format;
  unsigned long nitems, after;
  unsigned char *class_name = 0;
  unsigned char *wm_name = 0;

  if (!disp) 
    fprintf(out, "[ERROR] Could not open display \n\n");

  win = XDefaultRootWindow( disp );
  // should be removed later
  XSynchronize(disp, true);
  // set event mask, property changes
  XSelectInput(disp, win, PropertyChangeMask);

  while (XPending(disp)) {
    XNextEvent(disp, &xevent);
    if ( xevent.type == PropertyNotify ) {
      if ( !strcmp( XGetAtomName( disp, xevent.xproperty.atom ), "_NET_ACTIVE_WINDOW" ) ) {
        //is there a way to get the window deom rhw XEvent?
        if(XGetInputFocus(disp, &win, &revert)==0)
          fprintf(out, "noinputfocus \n\n");
        if (Success == XGetWindowProperty(disp, win, XInternAtom(disp,"WM_CLASS",false), 0, 65536, false, AnyPropertyType, &type, &format, &nitems, &after, &class_name)) {
          if (class_name) {
            fprintf(out, "\n[%s] ", class_name);
            XFree(class_name);
          }
          else
            fprintf(out, "\n[WM_CLASS-NODATA]");
        }
        else
          fprintf(out, "\n[WM_CLASS-NOSUCCESS]");

        //_NET_WM_NAME, WM_NAME, NET_WM_NAME
        if (Success == XGetWindowProperty(disp, win, XInternAtom(disp,"WM_NAME",false), 0, 65536, false, AnyPropertyType, &type, &format, &nitems, &after, &wm_name)) {
          if (wm_name) {
            fprintf(out, "[%s] > ", wm_name);
            XFree(wm_name);
          }
          else
            fprintf(out, "[WM_NAME-NODATA] > ");
        }
        else
          fprintf(out, "[WM_NAME-NOSUCCESS] > ");
      }
    }
    else{
      fprintf(out, "\n\nnot propertynotify \n\n");
    }
    fflush(out);
  }
  XCloseDisplay(disp); 
}
