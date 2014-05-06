package infoint.aufgabe1.name2country.guessing;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

import com.bleuelmedia.javatools.logging.Log;

public class GuessOperator {
	
	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "GuessOperator";
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	private File inputFile;
	private File outputFile;
	
	//==========================================================================
	// INSTANCE METHODS
	
	public GuessOperator(String infile, String outfile) {
		inputFile = new File(infile);
		outputFile = new File(outfile);
	}
	
	public void guessAll() {
		BufferedReader in;
		BufferedWriter out;
		
		//----------------------------------------------------------------------
		// Open reader and writer -> essential !!!
		
		try {
			in = new BufferedReader(new FileReader(inputFile));
			out = new BufferedWriter(new FileWriter(outputFile));
		} catch (FileNotFoundException ex) {
			Log.e(TAG, "Failed to open input file: " + ex.getMessage());
			return;
		} catch (IOException ex) {
			Log.e(TAG, "Failed to open output file: " + ex.getMessage());
			return;
		}
		
		//----------------------------------------------------------------------
		// Read input / guess / write output in try/catch-block
		
		try {
			String inputLine;
			while ((inputLine = in.readLine()) != null) {
//				ArrayList<String> qgrams = nameToQgrams(inputLine);
//				out.write(computeMostLikelyCountry(qgrams));
			}
			
			in.close();
			out.close();
		} catch (IOException ex) {
			Log.e(TAG, "Error while guessing the names: " + ex.getMessage());
			return;
		}
	}
	
}
