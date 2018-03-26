<?php
//if ($PHP_DB_CLASS != 1 )
//{
//	$PHP_DB_CLASS=1;
//	define("DB_OK",                         1);
//	define("DB_ERROR",                     -1);
//	define("DB_ERROR_SYNTAX",              -2);
//	define("DB_ERROR_CONSTRAINT",          -3);
//	define("DB_ERROR_NOT_FOUND",           -4);
//	define("DB_ERROR_ALREADY_EXISTS",      -5);
//	define("DB_ERROR_UNSUPPORTED",         -6);
//	define("DB_ERROR_MISMATCH",            -7);
//	define("DB_ERROR_INVALID",             -8);
//	define("DB_ERROR_NOT_CAPABLE",         -9);
//	define("DB_ERROR_TRUNCATED",          -10);
//	define("DB_ERROR_INVALID_NUMBER",     -11);
//	define("DB_ERROR_INVALID_DATE",       -12);
//	define("DB_ERROR_DIVZERO",            -13);
//	define("DB_ERROR_NODBSELECTED",       -14);
//	define("DB_ERROR_CANNOT_CREATE",      -15);
//	define("DB_ERROR_CANNOT_DELETE",      -16);
//	define("DB_ERROR_CANNOT_DROP",        -17);
//	define("DB_ERROR_NOSUCHTABLE",        -18);
//	define("DB_ERROR_NOSUCHFIELD",        -19);
//	define("DB_ERROR_NEED_MORE_DATA",     -20);
//	define("DB_ERROR_NOT_LOCKED",         -21);
//	define("DB_ERROR_VALUE_COUNT_ON_ROW", -22);
//	define("DB_ERROR_INVALID_DSN",        -23);
//	define("DB_ERROR_CONNECT_FAILED",     -24);
//	define("DB_ERROR_EXTENSION_NOT_FOUND",-25);
//	define("DB_ERROR_NOSUCHDB",           -25);
//	define("DB_ERROR_ACCESS_VIOLATION",   -26);
//
//    class DB
//    {
//
//		var $host;
//		var $dbname;
//		var $user;
//		var $passwd;
//		var $sql;
//		var $result;
//		var $value;
//		var $num;
//		var $handler;
//		var $status;
//
//		/*************************************
//		*
//		* connect
//		*
//		**************************************/
//		function connect($hostname, $user, $password , $dbname)
//		{
//			$this->host = $hostname;
//			$this->dbname = $dbname;
//			$this->user = $user;
//			$this->passwd = $password;
//
//			$this->handler = @mysql_connect( $this->host, $this->user, $this->passwd );
//			$this->status = @mysql_select_db( $this->dbname, $this->handler );
//
//			if(!$this->handler)
//			{
//				$err = $this->showerror();
//				return false;
//			}
//			return true;
//		}
//
//		function exec($sql)
//		{
//			$this->result = @mysql_query($sql);
//			return $this->result;
//		}
//		function num($res)
//		{
//			$this->num = @mysql_num_rows($res);
//
//			return $this->num;
//		}
//		function result($result,$num,$field)
//		{
//			$this->value = @mysql_result($result,$num,$field);
//			return $this->value;
//		}
//		function istable($tbname)
//		{
//			$this->listtable = @mysql_list_tables($this->dbname);
//			$total_count =  $this->num($this->listtable) ;
//			for ($i=0;$i<$total_count;$i++)
//			{
//				$table = @mysql_tablename($this->listtable,$i);
//				if ( $table == $tbname )
//					return 1;
//			}
//			return 0;
//		}
//		/*************************************
//		*
//		*
//		**************************************/
//		function close()
//		{
//			$err = @mysql_close($this->handler);
//
//		}
//
//
//		 /*************************************
//		 *
//		 *
//		 **************************************/
//		function errorMessage($value)
//		{
//			static $errorMessages;
//			if (!isset($errorMessages)) {
//				$errorMessages = array(
//					DB_ERROR                    => 'mysql error.',
//					DB_ERROR_ALREADY_EXISTS     => 'already exists',
//					DB_ERROR_CANNOT_CREATE      => 'can not create',
//					DB_ERROR_CANNOT_DELETE      => 'can not delete',
//					DB_ERROR_CANNOT_DROP        => 'can not drop',
//					DB_ERROR_CONSTRAINT         => 'constraint violation',
//					DB_ERROR_DIVZERO            => 'division by zero',
//					DB_ERROR_INVALID            => 'invalid',
//					DB_ERROR_INVALID_DATE       => 'invalid date or time',
//					DB_ERROR_INVALID_NUMBER     => 'invalid number',
//					DB_ERROR_MISMATCH           => 'mismatch',
//					DB_ERROR_NODBSELECTED       => 'no database selected',
//					DB_ERROR_NOSUCHFIELD        => 'no such field',
//					DB_ERROR_NOSUCHTABLE        => 'no such table',
//					DB_ERROR_NOT_CAPABLE        => 'DB backend not capable',
//					DB_ERROR_NOT_FOUND          => 'not found',
//					DB_ERROR_NOT_LOCKED         => 'not locked',
//					DB_ERROR_SYNTAX             => 'syntax error',
//					DB_ERROR_UNSUPPORTED        => 'not supported',
//					DB_ERROR_VALUE_COUNT_ON_ROW => 'value count on row',
//					DB_ERROR_INVALID_DSN        => 'invalid DSN',
//					DB_ERROR_CONNECT_FAILED     => '�ڷ���������� �����Ͽ����ϴ�.',
//					DB_OK                       => 'no error',
//					DB_WARNING                  => 'unknown warning',
//					DB_WARNING_READ_ONLY        => 'read only',
//					DB_ERROR_NEED_MORE_DATA     => 'insufficient data supplied',
//					DB_ERROR_EXTENSION_NOT_FOUND=> 'extension not found',
//					DB_ERROR_NOSUCHDB           => 'no such database',
//					DB_ERROR_ACCESS_VIOLATION   => 'insufficient permissions'
//				);
//			}
//
//			$errmessage = isset($errorMessages[$value]) ? $errorMessages[$value] : $errorMessages[DB_ERROR];
//		}
//
//		/**********************************
//		*
//		************************************/
//
//		function errorCode($errormsg)
//		{
//			static $error_regexps;
//			if (empty($error_regexps)) {
//				$error_regexps = array(
//					'/(Table does not exist\.|Relation [\"\'].*[\"\'] does not exist|sequence does not exist|class ".+" not found)$/' => DB_ERROR_NOSUCHTABLE,
//					'/Relation [\"\'].*[\"\'] already exists|Cannot insert a duplicate key into (a )?unique index.*/'      => DB_ERROR_ALREADY_EXISTS,
//					'/divide by zero$/'                     => DB_ERROR_DIVZERO,
//					'/pg_atoi: error in .*: can\'t parse /' => DB_ERROR_INVALID_NUMBER,
//					'/ttribute [\"\'].*[\"\'] not found$|Relation [\"\'].*[\"\'] does not have attribute [\"\'].*[\"\']/' => DB_ERROR_NOSUCHFIELD,
//					'/parser: parse error at or near \"/'   => DB_ERROR_SYNTAX,
//					'/referential integrity violation/'     => DB_ERROR_CONSTRAINT
//				);
//			}
//			foreach ($error_regexps as $regexp => $code) {
//				if (preg_match($regexp, $errormsg)) {
//					return $code;
//				}
//			}
//			// Fall back to DB_ERROR if there was no mapping.
//			return DB_ERROR;
//		}
//
//		/**********************************
//		*
//		*
//		************************************/
//
//		function showerror($errhandle = null)
//		{
//			if( $errhandle == null)
//				$err_code = DB_ERROR_CONNECT_FAILED;
//			else
//			{
//				$errmsg = mysql_errormessage($errhandle);
//				if($errmsg == null)
//					$err_code = DB_ERROR;
//				else
//					$err_code = $this->errorCode($errmsg);
//			}
//			$this->errorMessage($err_code);
//		}
//    }
//}
//?>