void ReverseString(char* string) 
 {  
      char r = '-';
      int lenx = 0;
      int a = 1;
      int count = 0;
      String StrUUID = "";
      Serial.println(string);
      
      String mystring(string); 
      mystring.replace("-", "");
      
      
      for (int i = 0; i <= mystring[lenx]; i++) {
       count++;
       if (count <= 9) {    

         if (count == 5) {StrUUID = StrUUID + r;}
         if (count == 7) {StrUUID = StrUUID + r;}
         if (count == 9) {StrUUID = StrUUID + r;}
         if (count == 11) {StrUUID = StrUUID + r;}
         
         char c = mystring[i + a]; 
         
        StrUUID = StrUUID + c;
         
         a = a - 1;
         c = mystring[i + a];
         
        StrUUID = StrUUID + c;
         
         a = a + 2; 
       }
       else
       {
        if (count <= 18) {     
          if (count == 9) {StrUUID = StrUUID + r;}
          if (count == 11) {StrUUID = StrUUID + r;}


         char c = mystring[i + a - 1]; 
         
        StrUUID = StrUUID + c;
         
         a = a - 1;
         c = mystring[i + a + 1];
         
        StrUUID = StrUUID + c;
         
         a = a + 2;        
        }
       }
    }
    Serial.println(StrUUID);
    
   int len = 0;
    while (StrUUID[len])
    len++;

    int down = 0;
    int up = len - 1;

    while (up > down)
    {
        char c = StrUUID[down];
       
        string[down++] = string[up];
        string[up--] = c;
    }
    Serial.println(string);
}