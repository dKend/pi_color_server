<?php
class Member {
	private $username;
	private $password;
	private $isValidated = false;
	private $id;
	
	public function __construct($uname, $pword)
	{
		$this->$username = $uname;
		$this->$password = $pword;
	}
	
	public function getUsername()
	{
		return($this->username);
	}
	
	public function getPassword()
	{
		return($this->password);
	}
	
	public function getId()
	{
		return($this->id);
	}
	public function getItValidated()
	{
		return($this->isValidated);
	}
	public function authLogin($connection)
	{
		if($this->isValidated === false)
		{
			//assumes errormode is set for connection and connection is a connection object (PDO)
			$statement = $connection->prepare(sprintf("SELECT member_password, member_id FROM member WHERE member_username = %s;", $this->getUsername()));
			$statement->execute();
			$result = $statement->fetch(PDO::FETCH_ASSOC);
			if( $this->getPassword === $result['member_password'] )
			{
				$this->isValidated = true;
				$this->id = $result['member_id']
			}
				
		}
		
		return($this->isValidated);
		
	}
}

?>