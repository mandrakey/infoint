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
	
	//==========================================================================
	// CLASS METHODS
	
	//==========================================================================
	// INSTANCE MEMBERS
	
	private File dbFile;
	private Connection connection;
	private boolean open;
	
	public Database(String path) {
		dbFile = new File(path);
	}
	
	public void open() {
		boolean create = (dbFile.exists()) ? false : true;
		
		try {
			connection = DriverManager.getConnection("jdbc:sqlite:" + dbFile.getAbsolutePath());
			open = true;
			if (create) {
				createDatabase();
			}
		} catch (SQLException ex) {
			Log.e(TAG, "Failed to open database at '" + dbFile.getAbsolutePath() + "'");
			return;
		}
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
	
	private boolean createDatabase() {
		try {
			Statement st = connection.createStatement();
			
			st.addBatch("BEGIN TRANSACTION;");
			st.addBatch("CREATE TABLE \"names\" "
					+ "(\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
					+ "\"name\" TEXT NOT NULL, "
					+ "\"country\" TEXT NOT NULL);");
			st.addBatch("CREATE TABLE \"qgrams\" "
					+ "(\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
					+ "\"qgram\" TEXT NOT NULL, "
					+ "\"names_id\" INTEGER NOT NULL);");
			st.addBatch("COMMIT;");
			
			st.executeBatch();
		} catch (SQLException ex) {
			Log.e(TAG, "Failed to create the database: " + ex.getMessage());
			return false;
		}
		
		return true;
	}
	
}
