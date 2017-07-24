void IPconversion() {
  
  IPAddress ip;
  ip.fromString(getParameter("IPaddr"));
  IPAddress mask;
  mask.fromString(getParameter("IPmask"));
  IPAddress gw;
  gw.fromString(getParameter("Gateway"));
  
  WiFi.config(ip,gw,mask);
}

