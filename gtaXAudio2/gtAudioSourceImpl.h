class gtAudioSourceImpl : public gtAudioSource{

	XAUDIO2_BUFFER			m_xaudio2Buffer;


public:

	gtAudioSourceImpl( void );
	~gtAudioSourceImpl( void );

	void updateData( void );	

	XAUDIO2_BUFFER&	getXAUDIO2_BUFFER( void ){
		return m_xaudio2Buffer;
	}

	f64 m_time;
};