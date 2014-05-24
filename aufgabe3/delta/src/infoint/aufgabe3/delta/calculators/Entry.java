package infoint.aufgabe3.delta.calculators;

import java.util.zip.CRC32;

import com.bleuelmedia.javatools.logging.Log;

public class Entry {
	
	//==========================================================================
	// CLASS MEMBERS
	
	private static CRC32 hasher;
	
	//==========================================================================
	// CLASS METHODS
	
	static {
		hasher = new CRC32();
	}

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
		//Log.d("Entry", "Add entry '" + key + "':'" + content + "'");
	}
	
}
