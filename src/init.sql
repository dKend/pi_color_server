CREATE TABLE members (
	member_id int(11) NOT NULL AUTO_INCREMENT,
	member_username varchar(30) NOT NULL,
	member_password varchar(50) NOT NULL,
	PRIMARY KEY (member_id),
	UNIQUE KEY (member_username)
);

CREATE TABLE  tags (
	tag_id int(11) NOT NULL AUTO_INCREMENT,
	tag_name varchar(30) NOT NULL,
	PRIMARY KEY (tag_id),
	UNIQUE KEY (tag_name)
);

CREATE TABLE fils ( 
	fil_id int(11) NOT NULL AUTO_INCREMENT,
	fil_path varchar(256) NOT NULL,
	member_id int(11) NULL,
	fil_time varchar(30) NULL,
	PRIMARY KEY( fil_id ),
	CONSTRAINT FK_fileMember FOREIGN KEY ( member_id ) REFERENCES members(member_id)
);

CREATE TABLE tag_to_files (
	ttf_id int(11) NOT NULL AUTO_INCREMENT,
	tag_id int(11) NOT NULL,
	fil_id int(11) NOT NULL,
	PRIMARY KEY (ttf_id),
	CONSTRAINT FK_t2fTags FOREIGN KEY (tag_id) REFERENCES tags(tag_id),
	CONSTRAINT FK_t2fFils FOREIGN KEY (fil_id) REFERENCES fils(fil_id)
);
