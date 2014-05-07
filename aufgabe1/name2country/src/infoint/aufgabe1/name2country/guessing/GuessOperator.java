package infoint.aufgabe1.name2country.guessing;

import infoint.aufgabe1.name2country.Database;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.bleuelmedia.javatools.logging.Log;
import com.sun.xml.internal.ws.util.StringUtils;

public class GuessOperator {
	
	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "GuessOperator";
	
	public static final Pattern RX_NAMES = Pattern.compile("([^ ]+)");
	
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
				ArrayList<String> qgrams = nameToQgrams(inputLine);
				
				Log.d(TAG, "QGRAMS for line '" + inputLine + "'");
				for (String s : qgrams) {
					System.out.print(s + " ");
				}
				System.out.println("");
				
				// Load countries
				ResultSet rs = getCountriesFromDb(qgrams);
				out.write(computeMostLikelyCountry(rs) + "\n");
			}
			
			in.close();
			out.close();
		} catch (IOException ex) {
			Log.e(TAG, "Error while guessing the names: " + ex.getMessage());
			return;
		}
	}
	
	/**
	 * Parse a name into qgrams.<br>
	 * Expects input line of the following format: <tt>Firstname Middlename 
	 * Lastname</tt>.
	 * @param line The line to parse.
	 * @return List of qgrams found in this line of text.
	 */
	private ArrayList<String> nameToQgrams(String line) {
		Matcher m = RX_NAMES.matcher(line);
		ArrayList<String> qgramsList = new ArrayList<>();
		
		while (m.find()) {
			if (m.groupCount() != 1) {
				Log.w(TAG, "Match '" + m.group(0) + "' has no matching group. "
						+ "(in line: " + line + "). Skipping.");
				continue;
			}
			
			qgramsList.addAll(qgrams(m.group(1)));
		}
		
		return qgramsList;
	}
	
	/**
	 * Divides an arbitrary string into 2-grams.
	 * @param text Text to divide.
	 * @return Vector of qgrams
	 */
	private ArrayList<String> qgrams(String text) {
		if (text == null || text.isEmpty()) {
			return null;
		}
		
		ArrayList<String> qgrams = new ArrayList<>();
		
		for (int i = 0; i < text.length() - 1; ++i) {
			qgrams.add("" + text.charAt(i) + text.charAt(i + 1));
		}
		
		return qgrams;
	}
	
	private ResultSet getCountriesFromDb(ArrayList<String> qgrams) {
		// Prepare question marks list
		StringBuilder qmarks = new StringBuilder();
		for (int i = 0; i < qgrams.size(); ++i) {
			qmarks.append("?" + ((i != qgrams.size() - 1) ? ", ": ""));
		}
		
		try {
			Database db = new Database();
			PreparedStatement pst = db.prepare("SELECT n.name, n.country, count(q.qgram) AS nr "
					+ "FROM qgrams AS q "
					+ "JOIN names AS n ON n.id=q.names_id "
					+ "WHERE q.qgram IN (" + qmarks.toString() + ") "
					+ "GROUP BY name "
					+ "HAVING nr > 2 "
					+ "ORDER BY nr DESC;");
			
			for (int i = 1; i <= qgrams.size(); ++i) {
				pst.setString(i, qgrams.get(i - 1));
			}
			ResultSet rs = pst.executeQuery();
			return rs;
		} catch (SQLException ex) {
			Log.w(TAG, "Failed to retrieve possible countries for " + Arrays.deepToString(qgrams.toArray())
					+ ": " + ex.getMessage());
			return null;
		}
	}
	
	private String computeMostLikelyCountry(ResultSet rs) {
		if (rs == null) {
			return "No match.";
		}
		
		try {
			HashMap<String, Integer> countryCounts = new HashMap<>();
			int maxCount = 0;
			String mostLikelyCountry = "";
			while (rs.next()) {
				String country = rs.getString("country");
				int count = ((countryCounts.containsKey(country))
						? countryCounts.get(country)
						: 0);
				
				countryCounts.put(country, ++count);
				
				if (count > maxCount) {
					maxCount = count;
					mostLikelyCountry = country;
				}
			}
			
			return mostLikelyCountry;
		} catch (SQLException ex) {
			Log.w(TAG, "Failed to process country counts result set: " + ex.getMessage());
			return "Error.";
		}
	}
	
}
