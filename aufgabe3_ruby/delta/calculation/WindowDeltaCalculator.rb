class WindowDeltaCalculator
    @@possibleDelims = [ ",", ";", ":", " " ]
    
    @inputBuffers = []
    @agingBuffers = []
    @delims = []
    @results = {}
    
    def calculateDelta(f1, f2)
        @results["UPDATE"] = 0
        @results["INSERT"] = 0
        @results["DELETE"] = 0
        
        # Open files
        in1 = open(f1, "r")
        in2 = open(f2, "r")
        
    end
    
    def fillInputBuffer(bufid, f)
        raise "Cannot load data from non-open file." if f.closed?
        
        @inputBuffers[bufid].clear
        while @inputBuffers[bufid].size < Delta::INPUTBUFFER_SIZE and !f.eof
            e = new Entry(f.getline, @delims[bufid])
            @inputBuffers[bufid][e.key] = e
        end
    end
    
    def matchBuffers(buf1, buf2)
        updates = 0
        toRemove = []
        
        for e in buf1
            
        end
    end
    
    def findDelimiters(f)
        raise "Cannot determine delimiter on closed file." if f.closed?
        
        line = f.getline
        quoted = false
        
        for i in 0..line.size
            if line[i] == "\""
                quoted = !quoted
                next
            end
            
            if @@possibleDelims.find_index(line[i]) != nil
                return line[i]
            end
        end
        
        raise "Failed to determine the delimiter."
    end
end
