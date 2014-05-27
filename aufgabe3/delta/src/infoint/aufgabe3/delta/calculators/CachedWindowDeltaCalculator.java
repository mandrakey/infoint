/* Copyright (C) 2014 Maurice Bleuel, Jan Siegert
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package infoint.aufgabe3.delta.calculators;

import infoint.aufgabe3.delta.Delta;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashMap;

import com.bleuelmedia.javatools.logging.Log;

public class CachedWindowDeltaCalculator implements DeltaCalculator {
	
	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "CachedWindowDeltaCalculator";
	
	private static int INPUTBUFFER_SIZE;
	private static String[] DELIMS;
	private static HashMap<File, HashMap<Long, Entry>> CACHE;
	
	//==========================================================================
	// CLASS METHODS
	
	static {
		INPUTBUFFER_SIZE = Delta.INPUTBUFFER_SIZE;
		
		DELIMS = new String[4];
		DELIMS[0] = ",";
		DELIMS[1] = ";";
		DELIMS[2] = " ";
		DELIMS[3] = ":";
		
		CACHE = new HashMap<>();
	}
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	private HashMap<Long, Entry> inputBuffer1;
	private HashMap<Long, Entry> inputBuffer2;
	private HashMap<Long, Entry> agingBuffer1;
	private HashMap<Long, Entry> agingBuffer2;
	private HashMap<String, Integer> results;
	
	//==========================================================================
	// INSTANCE METHODS
	
	public CachedWindowDeltaCalculator() {
		agingBuffer1 = new HashMap<>();
		agingBuffer2 = new HashMap<>();
	}

	@Override
	public HashMap<String, Integer> calculateDelta(File f1, File f2) {
		inputBuffer1 = new HashMap<>(INPUTBUFFER_SIZE);
		inputBuffer2 = new HashMap<>(INPUTBUFFER_SIZE);
		
		// Prepare result map
		results = new HashMap<>();
		results.put("INSERT", 0);
		results.put("UPDATE", 0);
		results.put("DELETE", 0);
		
		// Prepare cache
		synchronized(CACHE) {
			if (!CACHE.containsKey(f1)) {
				CACHE.put(f1, fileToCache(f1));
			}
			if (!CACHE.containsKey(f2)) {
				CACHE.put(f2, fileToCache(f2));
			}
			
			for (Entry e : CACHE.get(f1).values()) {
				inputBuffer1.put(e.key, e);
			}
			for (Entry e : CACHE.get(f2).values()) {
				inputBuffer2.put(e.key, e);
			}
		}
		
		matchBuffers(inputBuffer1, inputBuffer2);
		matchBuffers(inputBuffer1, agingBuffer2);
		matchBuffers(inputBuffer2, agingBuffer1);
		
		results.put("DELETE", inputBuffer1.size());
		results.put("INSERT", inputBuffer2.size());
		
		return results;
	}
	
	private void matchBuffers(HashMap<Long, Entry> buf1, HashMap<Long, Entry> buf2) {
		int updates = 0;
		
		ArrayList<Long> toRemove = new ArrayList<>();
		for (Entry e : buf1.values()) {
			if (buf2.containsKey(e.key)) {
				if (e.content != buf2.get(e.key).content) {
					++updates;
				}
				
				toRemove.add(e.key);
			}
		}
		
		for (long key : toRemove) {
			buf1.remove(key);
			buf2.remove(key);
		}
		
		results.put("UPDATE", results.get("UPDATE") + updates);
	}
	
	private String findDelimeter(String line) throws Exception {
		@SuppressWarnings("unused")
		String tmp[];
		
		for (String delim : DELIMS) {		
			if ((tmp = line.split(delim)).length > 1) {
				return delim;
			}
		}
		
		throw new Exception("Could not determine delimeter");
	}
	
	private HashMap<Long, Entry> fileToCache(File f) {
		try {
			BufferedReader in = new BufferedReader(new FileReader(f));
			String line;
			
			// Get delimeter
			line = in.readLine();
			String delim = findDelimeter(line);
			HashMap<Long, Entry> cache = new HashMap<>();
			while ((line = in.readLine()) != null) {
				Entry e = new Entry(line, delim);
				cache.put(e.key, e);
			}
			in.close();
			return cache;
		} catch (Exception ex) {
			Log.e(TAG, "Failed to cache file '" + f.getName() + "': " + ex.getMessage());
			return null;
		}
	}
	
	//==========================================================================
	// GETTER / SETTER
	
	public int getInputBufferSize() {
		return INPUTBUFFER_SIZE;
	}
	public void setInputBufferSize(int bufsize) {
		INPUTBUFFER_SIZE = (bufsize >= 1) ? bufsize : 100;
	}

}
