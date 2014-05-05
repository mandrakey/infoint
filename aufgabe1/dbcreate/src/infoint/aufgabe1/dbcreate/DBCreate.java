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
import java.util.HashMap;

import infoint.aufgabe1.dbcreate.parser.CSVParser;

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
		
		parser.addOption(helpOption).addOption(debugOption).addOption(pathOption);
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
		
		Log.DEBUG = DEBUG;
		
		//----------------------------------------------------------------------
		// Start parser
		
		CSVParser csvp = CSVParser.getInstance();
		try {
			csvp.parse(PATH);
		} catch (IOException ex) {
			Log.e(TAG, "Failed to parse the directory: " + ex.getMessage());
			return;
		}
		
		// Test: Output data
		/** @todo: Remove test output, replace with DB insertion */
		System.out.println("\n\nGot the following entries:\n====\n");
		HashMap<String, String> entries = csvp.getEntries();
		for (String name : entries.keySet()) {
			System.out.println("{ name: '" + name + "', country: '" + entries.get(name) + "' }");
		}
		
		System.out.println("====\nTotal: " + entries.size());
		
	}
	
}
