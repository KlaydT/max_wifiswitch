void IPconversion() {
  
  int Parts[4] = {0,0,0,0};
  int Part = 0;
    for ( int i=0; i<IPaddr.length(); i++ )
       {
       char c = IPaddr[i];
       if ( c == '.' )
          {
          Part++;
          continue;
          }
       Parts[Part] *= 10;
       Parts[Part] += c - '0';
      }
  IPAddress ip( Parts[0], Parts[1], Parts[2], Parts[3] );
  
  int Parts_mask[4] = {0,0,0,0};
  int Part_mask = 0;
    for ( int k=0; k<IPmask.length(); k++ )
       {
       char c_mask = IPmask[k];
       if ( c_mask == '.' )
          {
          Part_mask++;
          continue;
          }
       Parts_mask[Part_mask] *= 10;
       Parts_mask[Part_mask] += c_mask - '0';
      }
  IPAddress mask( Parts_mask[0], Parts_mask[1], Parts_mask[2], Parts_mask[3] );  

  int Parts_gw[4] = {0,0,0,0};
  int Part_gw = 0;
    for ( int l=0; l<Gateway.length(); l++ )
       {
       char c_gw = Gateway[l];
       if ( c_gw == '.' )
          {
          Part_gw++;
          continue;
          }
       Parts_gw[Part_gw] *= 10;
       Parts_gw[Part_gw] += c_gw - '0';
      }
  IPAddress gw( Parts_gw[0], Parts_gw[1], Parts_gw[2], Parts_gw[3] );  
  
  WiFi.config(ip,gw,mask);
}

