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

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.bleuelmedia.javatools.logging.Log;

/**
 * @brief Manages parsing of CSV files in a given directory.
 * Implemented as Singleton object because in our scenario, we only parse 
 * one directory at a time.
 * @author Maurice Bleuel <mandrakey@bleuelmedia.com>
 */
public class CSVParser {

	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "CSVParser";
	
	/** @brief The one-and-only CSVParser instance. */
	private static CSVParser instance;
	
	//==========================================================================
	// CLASS METHODS
	
	/**
	 * @brief Retrieve the CSVParser instance or create it, if necessary.
	 * @return The instance of CSVParser.
	 */
	public static CSVParser getInstance() {
		if (instance == null) {
			instance = new CSVParser();
		}
		return instance;
	}
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	/** @brief Regular expression for line matching. */
	private Pattern lineRx;
	/** @brief Regular expression for filename matching. */
	private Pattern fileRx;
	/** @brief Result HashMap containing Name to Country mappings. */
	private HashMap<String, String> entries;
	/** @brief Handles execution of file parser threads. */
	private ExecutorService exec;
	
	//==========================================================================
	// INSTANCE METHODS
	
	/**
	 * @brief Created the CSVParser instance.
	 * Defines regular expression values, creates an empty result HashMap and 
	 * creates a fixed thread pool with max-size=10.
	 */
	private CSVParser() {
		lineRx = Pattern.compile("^([^;]+);([^;]+);?.*$");
		fileRx = Pattern.compile(".*\\.csv$");
		entries = new HashMap<>();
		exec = Executors.newFixedThreadPool(10);
	}
	
	/**
	 * @brief Parse all matching files within given directory.
	 * Each file is parsed by an individual FileParser thread to speed up 
	 * processing.
	 * @param path Path to the directory.
	 * @throws IOException If directory does not exist, is no directory or 
	 * is not accessible.
	 */
	public void parse(String path) throws IOException {
		File f = new File(path);
		if (!f.isDirectory()) {
			throw new IOException("Path '" + path + "' is not a directory.");
		}
		Log.d(TAG, "Parsing files in '" + f.getPath() + "'...");
		
		for (File fi : f.listFiles()) {
			if (!fi.isFile()) {
				Log.d(TAG, "Skip file '" + fi.getName() + "'");
				continue;
			}
			
			Matcher m = fileRx.matcher(fi.getName());
			if (m.matches()) {
				Log.i(TAG, "Parse file '" + fi.getName() + "'...");
				exec.execute(new FileParser(fi));
			}
		}
		
		exec.shutdown();
		while (!exec.isTerminated()) {
		}
	}
	
	/**
	 * @brief Add a result entry to the map.
	 * Synchronized method to add detected entries to the result map. This 
	 * method is called by the individual FileParser threads to insert their 
	 * results.
	 * @param key Name to add
	 * @param value Corresponding country
	 * @todo Parse result names into q-grams, too
	 */
	public synchronized void addEntry(String key, String value) {
		if (!entries.containsKey(key)) {
			entries.put(key, value);
		}
	}
	
	//==========================================================================
	// GETTER / SETTER
	
	public Pattern getLineRx() {
		return lineRx;
	}
	
	public HashMap<String, String> getEntries() {
		return entries;
	}
	
}
