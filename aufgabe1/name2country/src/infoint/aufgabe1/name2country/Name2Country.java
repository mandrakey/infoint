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

package infoint.aufgabe1.name2country;

import infoint.aufgabe1.name2country.guessing.GuessOperator;

import java.io.File;
import java.io.FileNotFoundException;
import java.sql.SQLException;

import com.bleuelmedia.javatools.cliparser.CommandLineOption;
import com.bleuelmedia.javatools.cliparser.CommandLineParser;
import com.bleuelmedia.javatools.logging.Log;

public class Name2Country {

	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "Name2Country";
	
	private static String INFILE = "./in.txt";
	
	private static String OUTFILE = "./out.txt";
	
	private static String DBFILE = "./name2country.db3";
	
	private static boolean DEBUG = false;
	
	//==========================================================================
	// CLASS METHODS
	
	/**
	 * @brief Display usage information on standard out.
	 */
	public static void displayHelp() {
		System.out.println("Usage: name2country.jar -?|--help");
		System.out.println("Usage: name2country.jar [-d | --debug] [-o|--outfile] [-D|--dbfile] -i|--infile infile=./in.txt");
		System.out.println("help\t\tDisplay usage information");
		System.out.println("debug\t\tVerbose debug output (may be slow)");
		System.out.println("outfile\t\tSpecify file to write output to");
		System.out.println("\t\tdefault=./out.txt");
		System.out.println("dbfile\t\tSpecify location of the database file");
		System.out.println("\t\tdefault=./name2country.db3");
		System.out.println("infile\t\tSpecify file with input data");
	}
	
	public static void main(String[] args) {
		
		//----------------------------------------------------------------------
		// Parse command-line arguments
		
		CommandLineParser parser = new CommandLineParser();
		CommandLineOption infileOption = new CommandLineOption("i", "infile", "Specify the input filename");
		CommandLineOption outfileOption = new CommandLineOption("o", "outfile", "Specify the output filename");
		CommandLineOption dbfileOption = new CommandLineOption("D", "dbfile", "Specify location of the database file");
		CommandLineOption debugOption = new CommandLineOption("d", "debug", "Specify debug mode (slow)");
		CommandLineOption helpOption = new CommandLineOption("?", "help", "Display help");
		
		parser.addOption(infileOption).addOption(outfileOption).addOption(dbfileOption)
				.addOption(debugOption).addOption(helpOption);
		parser.addPositionalOption(infileOption, 1);
		parser.parse(args);
		
		if (parser.isSet(helpOption)) {
			displayHelp();
			return;
		}
		if (parser.isSet(debugOption)) {
			DEBUG = true;
		}
		if (parser.isSet(outfileOption)) {
			OUTFILE = parser.getArgumentsForOption(outfileOption).get(0).getValue().toString();
		}
		if (parser.isSet(dbfileOption)) {
			DBFILE = parser.getArgumentsForOption(dbfileOption).get(0).getValue().toString();
		}
		if (parser.isSet(infileOption)) {
			INFILE = parser.getArgumentsForOption(infileOption).get(0).getValue().toString();
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
		// Check files for existence
		
		File f = new File(INFILE);
		if (!f.exists()) {
			Log.e(TAG, "The specified input file '" + INFILE + "' does not "
					+ "exist or is not readable.");
			System.exit(1);
		}
		f = new File(DBFILE);
		if (!f.exists()) {
			Log.e(TAG, "The specified database file '" + DBFILE + "' does not "
					+ "exist or is not readable.");
			System.exit(1);
		}
		
		//----------------------------------------------------------------------
		// Try database connection
		
		try {
			new Database().open(DBFILE);
		} catch (SQLException ex) {
			Log.e(TAG, "Could not open the database: " + ex.getMessage());
			System.exit(1);
		}
		
		//----------------------------------------------------------------------
		// Do the guessing
		
		GuessOperator go = new GuessOperator(INFILE, OUTFILE);
		go.guessAll();
		
	}

}
