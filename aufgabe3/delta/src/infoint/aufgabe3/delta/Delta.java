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

import infoint.aufgabe3.delta.calculators.WindowDeltaCalculator;

import java.io.File;
import java.util.HashMap;

import com.bleuelmedia.javatools.logging.Log;

public class Delta {

	public static final String WORKDIR = "/local/II2014/uebung3/";
	public static final String OUTFILE = WORKDIR + "horst.txt";
	
	public static void main(String[] args) {
		Log.DEBUG = false;
		File f1 = new File("R5.csv");
		File f2 = new File("R4.csv");
		WindowDeltaCalculator c = new WindowDeltaCalculator();
		HashMap<String, Integer> res = c.calculateDelta(f1, f2);
		
		printResult(f1, f2, res);
	}
	
	private static void printResult(File f1, File f2, HashMap<String, Integer> result) {
		String[] fn1 = f1.getName().split("\\.");
		String[] fn2 = f2.getName().split("\\.");
		
		String res = fn1[0] + "," + fn2[0] + ","
				+ result.get("INSERT") + ","
				+ result.get("DELETE") + ","
				+ result.get("UPDATE");
		System.out.println(res);
	}
	
}
