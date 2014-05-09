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

package infoint.aufgabe1.dbcreate.parser;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.bleuelmedia.javatools.logging.Log;

/**
 * @brief Manages parsing of one single CSV file as a thread.
 * @author Maurice Bleuel <mandrakey@bleuelmedia.com>
 * @see java.lang.Thread
 */
public class FileParser extends Thread {

	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "FileParser";
	
	private static Pattern ABBRV_NAME_RX;
	
	//==========================================================================
	// CLASS MEMBERS
	
	static {
		ABBRV_NAME_RX = Pattern.compile(".+\\.");
	}
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	/** @brief File object to work on. */
	private File file;
	
	//==========================================================================
	// INSTANCE METHODS
	
	/**
	 * @brief Create a new file parser for a given file.
	 * @param file The file object to work on.
	 */
	public FileParser(File file) {
		this.file = file;
	}
	
	@Override
	/**
	 * @brief Override of the Thread run method: Parse the file.
	 * @throws FileNotFoundException
	 * @throws IOException
	 * @see java.lang.Thread.run()
	 */
	public void run() {
		try {
			CSVParser csvp = CSVParser.getInstance();
			Pattern lineRx = csvp.getLineRx();
			BufferedReader in = new BufferedReader(new FileReader(file));
			String line;
			
			while ((line = in.readLine()) != null) {
				Matcher m = lineRx.matcher(line);
				
				if (m.find() && m.groupCount() == 2) {
					Log.d(TAG, "Add {name: " + m.group(1) + ", country: " + m.group(2) 
							+ "} from '" + file + "'");
					String fullname = m.group(1).trim();
					String[] tmp = fullname.split(" ");
					for (String s : tmp) {
						Matcher abbrvMatcher = ABBRV_NAME_RX.matcher(s);
						if (s.length() > 2 || abbrvMatcher.find()) {
							FileEntry e = new FileEntry();
							e.qgrams = qgrams(s, 2);
							e.country = m.group(2).trim();
							e.fullname = fullname;
							csvp.addEntry(s, e);
						}
					}
				}
			}
			
			in.close();
		} catch (FileNotFoundException ex) {
			Log.e(TAG, "Could not find file '" + file + "'. Aborting.");
			return;
		} catch (IOException ex) {
			Log.e(TAG, "Failed to read from file '" + file + "'. Aborting.");
			return;
		}
	}
	
	/**
	 * @brief Divides an arbitrary string into qgrams of specified size.
	 * @param text Text to divide.
	 * @param size Size of the individual qgrams.
	 * @param fetchBlanks Fetch blanks at beginning and end of text (written as 
	 * # and $).
	 * @return Vector of qgrams
	 */
	private Vector<String> qgrams(String text, int size, boolean fetchBlanks) {
		if (text == null || text.isEmpty()) {
			return null;
		}
		
		Vector<String> v = new Vector<>();
		
		if (fetchBlanks) {
			v.add("##");
			v.add("#" + text.charAt(0));
		}
		
		for (int i = 0; i < text.length() - 1; ++i) {
			v.add("" + text.charAt(i) + text.charAt(i + 1));
		}
		
		if (fetchBlanks) {
			v.add(text.charAt(text.length() - 1) + "$");
			v.add("$$");
		}
		
		return v;
	}
	
	/**
	 * @brief Shortcut for qgrams.
	 * Implicitly sets fetchBlanks to FALSE.
	 * @param text Text to divide.
	 * @param size Size of qgrams.
	 * @return Vector of qgrams.
	 */
	private Vector<String> qgrams(String text, int size) {
		return qgrams(text, size, false);
	}
	
}
