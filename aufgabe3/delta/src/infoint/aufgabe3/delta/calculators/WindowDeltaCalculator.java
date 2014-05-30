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
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import com.bleuelmedia.javatools.logging.Log;

public class WindowDeltaCalculator implements DeltaCalculator {
	
	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "WindowDeltaCalculator";
	
	private static int inputBufferSize;
	private static String[] delims;
	
	//==========================================================================
	// CLASS METHODS
	
	static {
		inputBufferSize = Delta.INPUTBUFFER_SIZE;
		
		delims = new String[4];
		delims[0] = ",";
		delims[1] = ";";
		delims[2] = " ";
		delims[3] = ":";
	}
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	private HashMap<Long, Entry> inputBuffer1;
	private String delim1;
	private HashMap<Long, Entry> inputBuffer2;
	private String delim2;
	private HashMap<Long, Entry> agingBuffer1;
	private HashMap<Long, Entry> agingBuffer2;
	private HashMap<String, Integer> results;
	
	//==========================================================================
	// INSTANCE METHODS
	
	public WindowDeltaCalculator() {
		agingBuffer1 = new HashMap<>();
		agingBuffer2 = new HashMap<>();
	}

	@Override
	public HashMap<String, Integer> calculateDelta(File f1, File f2) {
		inputBuffer1 = new HashMap<>(inputBufferSize);
		inputBuffer2 = new HashMap<>(inputBufferSize);
		
		// Prepare result map
		results = new HashMap<>();
		results.put("INSERT", 0);
		results.put("UPDATE", 0);
		results.put("DELETE", 0);
		
		// Prepare buffers
		BufferedReader in1;
		BufferedReader in2;
		
		try {
			in1 = new BufferedReader(new FileReader(f1));
			in2 = new BufferedReader(new FileReader(f2));
			delim1 = findDelimeter(in1.readLine());
			delim2 = findDelimeter(in2.readLine());
		} catch (FileNotFoundException ex) {
			Log.e(TAG, "Cannot find a file: " + ex.getMessage());
			return null;
		} catch (Exception ex) {
			Log.e(TAG, "Problem while preparing: " + ex.getMessage());
			return null;
		}
		
		// Parse files, window mode
		fillInputBuffer(inputBuffer1, in1, delim1);
		fillInputBuffer(inputBuffer2, in2, delim2);
		while (!inputBuffer1.isEmpty() && !inputBuffer2.isEmpty()) {
			matchBuffers(inputBuffer1, inputBuffer2);
			matchBuffers(inputBuffer1, agingBuffer2);
			matchBuffers(inputBuffer2, agingBuffer1);
			
			for (Entry e : inputBuffer1.values()) {
				agingBuffer1.put(e.key, e);
			}
			for (Entry e : inputBuffer2.values()) {
				agingBuffer2.put(e.key, e);
			}
			
			fillInputBuffer(inputBuffer1, in1, delim1);
			fillInputBuffer(inputBuffer2, in2, delim2);
		}
		
		results.put("DELETE", agingBuffer1.size());
		results.put("INSERT", agingBuffer2.size());
		
		try {
			in1.close();
			in2.close();
		} catch (IOException ex) {
			Log.e(TAG, "Failed to close file streams: " + ex.getMessage());
		}
		
		return results;
	}
	
	private void fillInputBuffer(HashMap<Long, Entry> buffer, 
			BufferedReader reader, String delim) {
		String line;
		buffer.clear();
		
		try {
			while (buffer.size() < inputBufferSize 
					&& (line = reader.readLine()) != null) {
				Entry e = new Entry(line, delim);
				buffer.put(e.key, e);
			}
		} catch (IOException ex) {
			Log.e(TAG, "Failed to read line from file: " + ex.getMessage());
			return;
		}
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
		
		for (String delim : delims) {		
			if ((tmp = line.split(delim)).length > 1) {
				return delim;
			}
		}
		
		throw new Exception("Could not determine delimeter");
	}
	
	//==========================================================================
	// GETTER / SETTER
	
	public int getInputBufferSize() {
		return inputBufferSize;
	}
	public void setInputBufferSize(int bufsize) {
		inputBufferSize = (bufsize >= 1) ? bufsize : 100;
	}

}
