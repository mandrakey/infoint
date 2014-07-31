require "zlib"

class Entry

    def initialize(line, delim)
        tmp = line.split(delim)
        if tmp.length < 2
            raise "Line '" + line + "' cannot be parsed with delimiter '" + delim + "'"
        end
        
        line = line.gsub(delim, ";") if delim != ";"
        line = line.gsub("\"", "")
        
        @key = tmp[0]
        @content = Zlib::crc32(line).to_s
    end
    
    #===========================================================================
    # GETTER / SETTER
    
    def key()
        return @key
    end
    
    def content()
        return @content
    end

end
