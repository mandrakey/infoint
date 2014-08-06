require "thread"
require_relative "calculation/entry"
require_relative "calculation/WindowDeltaCalculator"

class Delta
    EXIT_OK = 0
    EXIT_NO_INPUT_FILES = 1
    EXIT_ERROR_WORKDIR = 11
    EXIT_ERROR_OUTFILE = 12
    
    WORKDIR = "./"
    OUTFILE = WORKDIR + "horst.txt"
    INPUTBUFFER_SIZE = 1000
    NUM_THREADS = 3
    
    @@results = {}
    @@doneFiles = []
    @@jobQueue = Queue.new
    @@deltaCount = 0
    @@resultLock = Mutex.new
    @@jobqueueLock = Mutex.new
    
    def self.getCsvFilesInDir(path)
        files = []
        Dir.foreach(path) { |f|
            files << f if f.match(/\.csv$/)
        }
        
        return files.sort
    end
    
    def self.resultKey(f1, f2)
        tmp = [ f1, f2 ]
        tmp = tmp.sort
        return tmp[0] + "," + tmp[1]
    end
    
    def self.resultsExist(f1, f2)
        return (@@doneFiles.find_index(resultKey(f1,f2)) != nil) ? true : false
    end
    
    def self.addResultsForEquals(f1, f2)
        res = { "UPDATE" => 0, "INSERT" => 0, "DELETE" => 0 }
        addResults(f1, f2, res)
    end
    
    def self.addResults(f1, f2, res)
        @@resultLock.synchronize {
            @@results[resultKey(f1, f2)] = res
        }
    end
    
    def self.addFromExistingResults(f1, f2, key)
        @@resultLock.synchronize {
            @@results[resultKey(f1, f2)] = @@results[key]
        }
    end
    
    def self.printResult(f1, f2, res)
        return f1 + "," + f2 + "," + 
            res["INSERT"].to_s + "," + 
            res["DELETE"].to_s + "," + 
            res["UPDATE"].to_s
    end
    
    #---------------------------------------------------------------------------
    # GETTER / SETTER
    
    def self.results()
        return @@results
    end
    
    def self.addJob(job)
        @@jobQueue << job
    end
    
    def self.raiseDeltaCount
        @@deltaCount += 1
    end
    
    def self.deltaCount()
        return @@deltaCount
    end
    
    def self.addDoneFiles(key)
        @@doneFiles << key
    end

end

#===============================================================================
# PROCESSING

files = Delta::getCsvFilesInDir(Delta::WORKDIR)
if files.length == 0
    puts "No input files. Exiting."
    exit Delta::EXIT_NO_INPUT_FILES
end

#-------------------------------------------------------------------------------
# Run delta computation for all files

resultsToCopy = []
for f1 in files
    for f2 in files
        
        if Delta::resultsExist(f1, f2)
            puts "Results for " + Delta::resultKey(f1,f2) + " already done."
            resultsToCopy << [ f1, f2 ]
            next
        end
        
        if f1 == f2
            Delta::addResultsForEquals(f1, f2)
        else
            Delta::addJob([f1, f2])
            Delta::raiseDeltaCount
            Delta::addResultsForEquals(f1, f2) # Remove !!
        end
        
        Delta::addDoneFiles(Delta::resultKey(f1, f2))
        
    end
end

# Run threads
threads = []
for i in 0..Delta::NUM_THREADS
    #threads[i] = Thread.new { Delta::startCalculator }
end
for t in threads
    t.join
end

# Copy missing results
for p in resultsToCopy
    Delta::addFromExistingResults(p[0], p[1], Delta::resultKey(p[0],p[1]))
end

# Print results
results = Delta::results
for f1 in files
    for f2 in files
        puts Delta::printResult(f1, f2, results[Delta::resultKey(f1, f2)])
    end
end

puts "Calculated " + Delta::deltaCount.to_s + " out of " + (files.length**2).to_s + 
    " deltas."
