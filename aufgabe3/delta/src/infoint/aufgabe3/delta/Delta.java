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

package infoint.aufgabe3.delta;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.bleuelmedia.javatools.logging.Log;
import com.bleuelmedia.javatools.util.Pair;

public class Delta {

	public static final String TAG = "Delta";
	
	//==========================================================================
	// RETURN CODES
	
	public static final int EXIT_OK = 0;
	public static final int EXIT_NO_INPUT_FILES = 1;
	public static final int EXIT_ERROR_WORKDIR = 11;
	public static final int EXIT_ERROR_OUTFILE = 12;
	
	//==========================================================================
	// CONFIGURATION
	
	public static final String WORKDIR = "/local/II2014/uebung3/";
	//public static final String WORKDIR = "./";
	public static final String OUTFILE = WORKDIR + "horst.txt";
	public static final int INPUTBUFFER_SIZE = 10000;
	public static final int NUM_THREADS = 12;
	
	private static HashMap<String, HashMap<String, Integer>> results;
	private static ArrayList<String> DONE_FILES;
	private static int DELTA_COUNT = 0;
	
	public static void main(String[] args) {
		Log.DEBUG = true;
		
		results = new HashMap<>();
		DONE_FILES = new ArrayList<>();
		
		File dir = new File(WORKDIR);
		if (!dir.isDirectory()) {
			Log.e(TAG, "The defined working directory '" + WORKDIR + "' does "
					+ "not exist or is no directory.");
			System.exit(EXIT_ERROR_WORKDIR);
		}
		
		File[] files = dir.listFiles(new CSVFileFilter());
		Arrays.sort(files);
		if (files.length == 0) {
			Log.i(TAG, "No input files. Exiting.");
			System.exit(EXIT_NO_INPUT_FILES);
		}
		
		//----------------------------------------------------------------------
		// Run delta computation for all files
		ExecutorService exec = Executors.newFixedThreadPool(NUM_THREADS);
		ArrayList<Pair<File, File>> resultsToCopy = new ArrayList<>();
		for (File f1 : files) {
			for (File f2 : files) {
				if (resultsExist(f1, f2)) {
					Log.i(TAG, "Results for " + resultKey(f1, f2) + "' already done.");
					resultsToCopy.add(new Pair<>(f1, f2));
					continue;
				}
				
				if (f1.getAbsolutePath().equals(f2.getAbsolutePath())) {
					addResultForEquals(f1, f2);
				} else {
					exec.execute(new DeltaCalculationWorker(f1, f2));
					++DELTA_COUNT;
				}
				
				DONE_FILES.add(resultKey(f1,f2));
			}
		}
		
		exec.shutdown();
		while (!exec.isTerminated()) {
		}
		
		// Copy missing results
		for (Pair<File, File> p : resultsToCopy) {
			File f1 = p.getFirst();
			File f2 = p.getSecond();
			addFromExistingResults(f1, f2, f2.getName() + "," + f1.getName());
		}
		
		try {
			BufferedWriter out = new BufferedWriter(new FileWriter(new File(OUTFILE)));
			for (File f1 : files) {
				for (File f2 : files) {
					String key = f1.getName() + "," + f2.getName();
					out.write(printResult(f1, f2, results.get(key)) + "\n");
				}
			}
			
			out.flush();
			out.close();
		} catch (IOException ex) {
			Log.e(TAG, "Error writing results to file: " + ex.getMessage());
			System.exit(EXIT_ERROR_OUTFILE);
		}
		
		Log.i(TAG, "Calculated " + DELTA_COUNT + " out of "
				+ (int)Math.pow(files.length, 2) + " deltas.");
	}
	
	public static boolean resultsExist(File f1, File f2) {
		return DONE_FILES.contains(resultKey(f1,f2));
	}
	
	public static synchronized void addResults(File f1, File f2, HashMap<String, Integer> result) {
		results.put(f1.getName() + "," + f2.getName(), result);
	}
	
	private static String printResult(File f1, File f2, HashMap<String, Integer> result) {
		String[] fn1 = f1.getName().split("\\.");
		String[] fn2 = f2.getName().split("\\.");
		
		String res = fn1[0] + "," + fn2[0] + ","
				+ result.get("INSERT") + ","
				+ result.get("DELETE") + ","
				+ result.get("UPDATE");
		return res;
	}
	
	private static void addResultForEquals(File f1, File f2) {
		HashMap<String, Integer> res = new HashMap<>();
		res.put("UPDATE", 0);
		res.put("DELETE", 0);
		res.put("INSERT", 0);
		Delta.addResults(f1, f2, res);
	}
	
	private static void addFromExistingResults(File f1, File f2, String key) {
		HashMap<String, Integer> original = results.get(key);
		HashMap<String, Integer> add = new HashMap<>();
		
		add.put("UPDATE", original.get("UPDATE"));
		add.put("INSERT", original.get("DELETE"));
		add.put("DELETE", original.get("INSERT"));
		results.put(f1.getName() + "," + f2.getName(), add);
	}
	
	private static  String resultKey(File f1, File f2) {
		String[] tmp = { f1.getName(), f2.getName() };
		Arrays.sort(tmp);
		return tmp[0] + "," + tmp[1];
	}
	
}
