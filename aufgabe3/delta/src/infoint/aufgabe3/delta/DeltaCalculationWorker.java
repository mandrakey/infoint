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

import infoint.aufgabe3.delta.calculators.DeltaCalculator;
import infoint.aufgabe3.delta.calculators.WindowDeltaCalculator;

import java.io.File;
import java.util.HashMap;

import com.bleuelmedia.javatools.logging.Log;

public class DeltaCalculationWorker implements Runnable {

	public static final String TAG = "DeltaCalculationWorker";
	
	private File f1;
	private File f2;
	
	public DeltaCalculationWorker(File f1, File f2) {
		this.f1 = f1;
		this.f2 = f2;
	}
	
	@Override
	public void run() {
		Log.i(TAG, "Begin " + f1.getName() + "," + f2.getName());
		DeltaCalculator calc = new WindowDeltaCalculator();
		
		HashMap<String, Integer> res = calc.calculateDelta(f1, f2);
		Delta.addResults(f1, f2, res);
		Log.i(TAG, "Finish " + f1.getName() + "," + f2.getName());
	}

}
