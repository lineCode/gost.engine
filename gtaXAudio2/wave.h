struct Wave{
	
	Wave( void ):
		/*m_pwfx( nullptr ),*/
		m_dwSize( 0 ),
		m_dwFlags( 0 ),
		m_bIsReadingFromMemory( FALSE ),
		m_pbDataCur( nullptr )
	{
		ZeroMemory( &m_ck, sizeof( m_ck ) );
		ZeroMemory( &m_ckRiff, sizeof( m_ckRiff ) );
		ZeroMemory( &m_mmioinfoOut, sizeof( m_mmioinfoOut ) );
	}

	~Wave( void ){
		/*SAFE_DELETE( m_pwfx );*/
	}

	gtAudioSourceImpl* read( const gtString& fileName );

		/// Pointer to WAVEFORMATEX structure
	/*WAVEFORMATEX* m_pwfx;*/

		/// MM I/O handle for the WAVE
	HMMIO m_hmmio;       

		/// Multimedia RIFF chunk
	MMCKINFO m_ck;

		/// Use in opening a WAVE file
	MMCKINFO m_ckRiff;

		/// The size of the wave file
	DWORD m_dwSize;

	MMIOINFO m_mmioinfoOut;

	DWORD m_dwFlags;

	BOOL m_bIsReadingFromMemory;

	u8* m_pbDataCur;
};