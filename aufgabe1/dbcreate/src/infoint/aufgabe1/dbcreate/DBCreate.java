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

package infoint.aufgabe1.dbcreate;

import java.io.IOException;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;

import infoint.aufgabe1.dbcreate.parser.CSVParser;
import infoint.aufgabe1.dbcreate.parser.FileEntry;
import infoint.aufgabe1.dbcreate.Database;

import com.bleuelmedia.javatools.cliparser.CommandLineOption;
import com.bleuelmedia.javatools.cliparser.CommandLineParser;
import com.bleuelmedia.javatools.logging.Log;

/**
 * @brief Main program class with entry point and preparation.
 * @author Maurice Bleuel <mandrakey@bleuelmedia.com>
 */
public class DBCreate {

	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "DBCreate";
	
	/** 
	 * @brief Determines wether debug output is enabled or not.
	 * Derived from command-line argument "-d|--debug".
	 */
	public static boolean DEBUG = false;
	
	/**
	 * @brief Contains the path where the to-be-parsed CSV-files are located.
	 * Derived from command-line argument "-p|--path".
	 */
	public static String PATH = ".";
	
	public static String DBFILE = "./name2country.db3";
	
	//==========================================================================
	// CLASS METHODS
	
	/**
	 * @brief Display usage information on standard out.
	 */
	public static void displayHelp() {
		System.out.println("Usage: dbcreate.jar [-d | --debug] [-p | --path] path=./\n");
		System.out.println("debug\t\tDisplay usage information");
		System.out.println("path\t\tPath to directory containing files to parse");
	}
	
	/**
	 * @brief Program entry point.
	 * @param args Command line arguments.
	 */
	public static void main(String[] args) {
		
		//----------------------------------------------------------------------
		// Parse commandline arguments
		
		CommandLineParser parser = new CommandLineParser();
		CommandLineOption helpOption = new CommandLineOption("?", "help", "Display usage information");
		CommandLineOption debugOption = new CommandLineOption("d", "debug", "Display debug output");
		CommandLineOption pathOption = new CommandLineOption("p", "path", "Path to directory containing files to parse");
		CommandLineOption outputOption = new CommandLineOption("o", "output", "Path to write result database file to");
		
		parser.addOption(helpOption).addOption(debugOption).addOption(pathOption)
				.addOption(outputOption);
		parser.addPositionalOption(pathOption, 1);
		parser.parse(args);
		
		if (parser.isSet(helpOption)) {
			displayHelp();
			return;
		}
		
		DEBUG = parser.isSet(debugOption) ? true : false;
		if (parser.isSet(pathOption)) {
			PATH = parser.getArgumentsForOption(pathOption).get(0).getValue().toString();
		}
		if (parser.isSet(outputOption)) {
			DBFILE = parser.getArgumentsForOption(outputOption).get(0).getValue().toString();
		}
		
		Log.DEBUG = DEBUG;
		
		//----------------------------------------------------------------------
		// Load database driver
		
		try {
			Class.forName("org.sqlite.JDBC");
		} catch (ClassNotFoundException ex) {
			Log.e(TAG, "Could not load SQLite driver: " + ex.getMessage());
			System.exit(1);
		}
		
		//----------------------------------------------------------------------
		// Start parser
		
		CSVParser csvp = CSVParser.getInstance();
		try {
			csvp.parse(PATH);
		} catch (IOException ex) {
			Log.e(TAG, "Failed to parse the directory: " + ex.getMessage());
			return;
		}
		
		//----------------------------------------------------------------------
		// Write data to database
		
		HashMap<String, FileEntry> entries = csvp.getEntries();
		System.out.println("Got total of " + entries.size() + " entries.");
		
		System.out.println("\n====\nWriting data to database ...\n");
		writeDbData();
		
	}
	
	private static void writeDbData() {
		try {
			Database db = new Database(DBFILE);
			db.open();
			
			CSVParser csvp = CSVParser.getInstance();
			HashMap<String, FileEntry> entries = csvp.getEntries();
			
			Statement st = db.createStatement();
			st.execute("PRAGMA synchronous = OFF;");
			st.execute("PRAGMA journal_mode = MEMORY;");
			st.execute("CREATE TEMPORARY TABLE tmp (last_name_id INTEGER NOT NULL);");
			
			PreparedStatement insertNames = db.prepare("INSERT INTO names (name,country) VALUES (?,?)");
			PreparedStatement insertQgrams = db.prepare("INSERT INTO qgrams (qgram,names_id) VALUES (?, (SELECT last_name_id FROM tmp LIMIT 1))");
			
			st.execute("INSERT INTO tmp VALUES (0);");
			
			int nr = 0;
			st.execute("BEGIN TRANSACTION;");
			for (String name : entries.keySet()) {
				//Log.i(TAG, "Adding entry '" + name + "' ...");
				FileEntry e = entries.get(name);
				
				// Add name and save name id
				insertNames.setString(1, name);
				insertNames.setString(2, e.country);
				insertNames.execute();
				st.execute("UPDATE tmp SET last_name_id=last_insert_rowid();");
				
				// Now add all qgrams
				for (String qgram : e.qgrams) {
					insertQgrams.setString(1, qgram);
					insertQgrams.execute();
				}
				
				if (++nr % 25 == 0) {
					Log.i(TAG, "[" + nr + " / " + entries.size() + "].");
				}
			}
			
			st.execute("COMMIT;");
			st.execute("DROP TABLE tmp;");
			st.execute("PRAGMA synchronous = ON;");
			st.execute("PRAGMA journal_mode = DELETE;");
			Log.i(TAG, "Done.");
			
		} catch (SQLException ex) {
			Log.e(TAG, "Failed to insert data: " + ex.getMessage());
		}
	}
	
}
