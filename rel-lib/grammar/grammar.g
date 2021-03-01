// Grammar for .rs file

/* 
Keywords:
TYPE    'type'       a type definition
ENUM    'enum'       an enum definition
ID      'id'         a unique identifier: [a-zA-Z0-9_]+
INT     'int'        a signed integer value
STRING  'string'     a string value
LINK    'link'       a link definition

Operators and Symbols:
           ,           comma delimiter
           :           colon
CMT_START  /*          begin of multi-line commment
CMT_END    */          end of multi-line comment
CMT_LINE   //          inline commment
           {           begin of type or enum block
           }           end of type or enum
EOL        \n or \r\n  end of line
*/

multi_line_comment ::= CMT_START [.]* CMT_END
line_comment ::= CMT_LINE [.]* EOL
                       
type_definition ::= TYPE identifier { 
    [identifier : (ID | STRING | LINK | INT | enum_definition),]+
}

enum_definition ::= ENUM identifier {
    [identifier,]+
}

identifier ::= [a-zA-Z_\.][a-zA-Z0-9_\.]*




// Grammar for .rd file

/*
Operators and Symbols:
                        ,           comma delimiter
                        :           colon
CMT_START               /*          begin of multi-line commment
CMT_END                 */          end of multi-line comment
CMT_LINE                //          inline commment
                        {           begin of type instance
                        }           end of type instance
EOL                     \n or \r\n  end of line
QUOTATION_MARK          "           quotation mark, indicating begin and end of string
QUOTATION_MARK_MASKED   \"          quotation mark, masked, to use it within strings
ARRAY_BEGIN             [           begin of an array of links
ARRAY_END               ]           end of an array of links

multi_line_comment ::= CMT_START [.]* CMT_END
line_comment ::= CMT_LINE [.]* EOL

type_instance ::= identifier {
    [identifier : (string | enum_element | integer | link | identifier),]+ 
}

link ::= identifier
link ::= ARRAY_BEGIN [identifier,]+ ARRAY_END

integer ::= [0-9]+

enum_element ::= identifier

string ::= QUOTATION_MARK [QUOTATION_MARK_MASKED | .]* QUOTATION_MARK

identifier ::= [a-zA-Z_\.][a-zA-Z0-9_\.]*
