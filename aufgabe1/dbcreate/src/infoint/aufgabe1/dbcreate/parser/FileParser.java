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
					String[] tmp = m.group(1).trim().split(" ");
					for (String s : tmp) {
						if (s.length() > 2) {
							csvp.addEntry(s, m.group(2).trim());
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
	
}
