package sub.app.client;

import sub.app.entry.CTypeCast;

public class Contact implements Packet{

	long id = 0;
	int times_contacted = 0;
	long last_time_contacted = 0;
	int starred = 0;
	int type_addr = 0;

	String display_name = "";
	String phones = "";
	String emails = "";
	String notes = "";
	String street = "";
	String city = "";
	String region = "";
	String postalcode = "";
	String country = "";
	String messaging = "";
	String OrganisationName = "";
	String OrganisationStatus = "";
	byte[] photo = null;
	
	public void checknull()
	{
		if(display_name==null) display_name = "";
		if(phones==null) phones = "";
		if(emails==null) emails = "";
		if(notes==null) notes = "";
		if(street==null) street = "";
		if(city==null) city = "";
		if(region==null) region = "";
		if(postalcode==null) postalcode = "";
		if(country==null) country = "";
		if(messaging==null) messaging = "";
		if(OrganisationName==null) OrganisationName = "";
		if(OrganisationStatus==null) OrganisationStatus = "";
	}

	public int getBodySize() {
		
		checknull();
		
		int nSize = 0;
		nSize = 8 * 2 + 4 * 3;
		
		nSize = nSize + display_name.length() * 2 + 1;
		nSize = nSize + phones.length() * 2 + 1;
		nSize = nSize + emails.length() * 2 + 1;
		nSize = nSize + notes.length() * 2 + 1;
		nSize = nSize + street.length() * 2 + 1;
		nSize = nSize + city.length() * 2 + 1;
		nSize = nSize + region.length() * 2 + 1;
		nSize = nSize + postalcode.length() * 2+ 1;
		nSize = nSize + country.length() * 2 + 1;
		nSize = nSize + messaging.length() * 2 + 1;
		nSize = nSize + OrganisationName.length() * 2 + 1;
		nSize = nSize + OrganisationStatus.length() * 2+ 1;
		
		if(photo == null)
		{
			nSize = nSize + 4;
		}
		else
		{
			nSize = nSize + photo.length + 4;
		}

		return nSize + 4;
	}

	public byte[] build() {
		
		int nSize = getBodySize();
		
		CTypeCast  bb = CTypeCast.allocate(nSize);
		
		bb.putInt(nSize);
		
		bb.putLong(id);
		bb.putInt(times_contacted);
		bb.putLong(last_time_contacted);
		bb.putInt(starred);
		bb.putInt(type_addr);

		bb.putString(display_name);
		bb.putString(phones);
		bb.putString(emails);
		bb.putString(notes);
		bb.putString(street);
		bb.putString(city);
		bb.putString(region);
		bb.putString(postalcode);
		bb.putString(country);
		bb.putString(messaging);
		bb.putString(OrganisationName);
		bb.putString(OrganisationStatus);
		
		bb.putBytes(photo);

		return bb.toPtr();
	}

	public void parse(byte[] packet) {
		// TODO Auto-generated method stub
	}

	public Contact() {
		
	}

	public String getRegion() {
		return region;
	}
	
	public void setRegion(String reg) {
		region = reg;
	}
	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public int getTimes_contacted() {
		return times_contacted;
	}

	public void setTimes_contacted(int times_contacted) {
		this.times_contacted = times_contacted;
	}

	public long getLast_time_contacted() {
		return last_time_contacted;
	}

	public void setLast_time_contacted(long last_time_contacted) {
		this.last_time_contacted = last_time_contacted;
	}

	public String getDisplay_name() {
		return display_name;
	}

	public void setDisplay_name(String display_name) {
		this.display_name = display_name;
	}

	public int getStarred() {
		return starred;
	}

	public void setStarred(int starred) {
		this.starred = starred;
	}

	public String getPhones() {
		return phones;
	}

	public void setPhones(String phones) {
		this.phones = phones;
	}

	public String getEmails() {
		return emails;
	}

	public void setEmails(String emails) {
		this.emails = emails;
	}

	public String getNotes() {
		return notes;
	}

	public void setNotes(String notes) {
		this.notes = notes;
	}

	public String getStreet() {
		return street;
	}

	public void setStreet(String street) {
		this.street = street;
	}

	public String getCity() {
		return city;
	}

	public void setCity(String city) {
		this.city = city;
	}

	public String getPostalcode() {
		return postalcode;
	}

	public void setPostalcode(String postalcode) {
		this.postalcode = postalcode;
	}

	public String getCountry() {
		return country;
	}

	public void setCountry(String country) {
		this.country = country;
	}

	public int getType_addr() {
		return type_addr;
	}

	public void setType_addr(int type_addr) {
		this.type_addr = type_addr;
	}

	public String getMessaging() {
		return messaging;
	}

	public void setMessaging(String messaging) {
		this.messaging = messaging;
	}

	public String getOrganisationName() {
		return OrganisationName;
	}

	public void setOrganisationName(String organisationName) {
		OrganisationName = organisationName;
	}

	public String getOrganisationStatus() {
		return OrganisationStatus;
	}

	public void setOrganisationStatus(String organisationStatus) {
		OrganisationStatus = organisationStatus;
	}

	public byte[] getPhoto() {
		return photo;
	}

	public void setPhoto(byte[] photo) {
		this.photo = photo;
	}
}
