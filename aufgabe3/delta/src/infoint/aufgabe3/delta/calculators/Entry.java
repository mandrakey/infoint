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

package infoint.aufgabe3.delta.calculators;

public class Entry {

	//==========================================================================
	// INSTANCE MEMBERS
	
	public long key;
	public long content;
	public String original;
	
	//==========================================================================
	// INSTANCE METHODS
	
	public Entry(String line, String delim) throws IllegalArgumentException {
		if (!delim.equals(";")) {
			line = line.replace(delim, ";");
		}
		line = line.replace("\"", "");
		
		String tmp[] = line.split(";");
		if (tmp.length < 2) {
			throw new IllegalArgumentException("Line '" + line + "' can not "
					+ "be parsed with delimiter '" + delim + "'.");
		}
		
		key = Long.parseLong(tmp[0]);
		
		content = line.hashCode();
		original = line;
	}
	
}
