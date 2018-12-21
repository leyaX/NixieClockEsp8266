
// http://www.cplusplus.com/reference/cstdio/printf/

void dbgprint ( const char *format, ... )
{
    static char sbuf[100];                                                     // For debug lines
    va_list varArgs;                                                            // For variable number of params

    va_start ( varArgs, format );                                               // Prepare parameters
    vsnprintf ( sbuf, sizeof ( sbuf ), format, varArgs );                       // Format the message
    va_end ( varArgs );                                                         // End of using parameters
    
    if  ( DEBUG )                                                               // DEBUG on?
      {
        Serial.print ( sbuf );
      }
}
