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

import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import com.bleuelmedia.javatools.logging.Log;

public class Database {

	//==========================================================================
	// CLASS MEMBERS
	
	public static final String TAG = "Database";
	
	// Monostate members
	private static File dbFile;
	private static Connection connection;
	private static boolean open;
	
	//==========================================================================
	// CLASS METHODS
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	public void open(String path) throws SQLException {
		if (open) {
			close();
		}
		
		dbFile = new File(path);
		connection = DriverManager.getConnection("jdbc:sqlite:" + dbFile.getAbsolutePath());
		open = true;
	}
	
	public void close() {
		if (!open) {
			return;
		}
		
		try {
			connection.close();
		} catch (SQLException ex) {
			Log.e(TAG, "Failed to close database: " + ex.getMessage());
			System.exit(1);
		}
		open = false;
	}
	
	public PreparedStatement prepare(String sql) throws SQLException {
		return connection.prepareStatement(sql);
	}
	
	public Statement createStatement() throws SQLException {
		return connection.createStatement();
	}
	
}
