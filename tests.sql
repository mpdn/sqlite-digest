.load ./digest.so

CREATE TABLE tests (
	input TEXT,
	sha TEXT,
	md TEXT,
	gen_md TEXT GENERATED ALWAYS AS (hex(digest('md5', input))) STORED
);

INSERT INTO tests VALUES 
	("abc", "A9993E364706816ABA3E25717850C26C9CD0D89D", "900150983CD24FB0D6963F7D28E17F72"),
	("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983E441C3BD26EBAAE4AA1F95129E5E54670F1", "8215EF0796A20BCAAAE116D3876C664A");

SELECT input FROM tests WHERE
	hex(sha1(input)) != sha OR
	hex(md5(input)) != md OR
	hex(digest("sha1", input)) != sha OR
	gen_md != md;
