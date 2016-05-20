#ifndef _PDF_READER_LINK_H
#define _PDF_READER_LINK_H

#include "Object.h"

namespace PdfReader
{
	class StringExt;
	class Array;
	class Dict;

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkAction
	//-------------------------------------------------------------------------------------------------------------------------------

	enum LinkActionType
	{
		actionGoTo,        // Go to destination
		actionGoToR,       // Go to destination � ����� �����
		actionLaunch,      // ��������� ���������� (��� ��������� ��������)
		actionURI,         // URI
		actionNamed,       // Named
		actionMovie,       // Movie
		actionUnknown      // 
	};

	class LinkAction
	{
	public:

		virtual ~LinkAction() {}

		// ��������� �� �������� ������ LinkAction?
		virtual bool CheckValidate() = 0;

		// ��� LinkAction.
		virtual LinkActionType GetType() = 0;

		// ������ Destination (������ ������� �ction) Name, String ��� Array.
		static LinkAction *ParseDestination(Object *pObject);

		// ������ Action dictionary.
		static LinkAction *ParseAction(Object *pObject, StringExt *seBaseURI = NULL);

		// ������� �������� ����� ��� File specification.
		static StringExt *GetFileSpecName(Object *pFileSpecObject);
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkDestination
	//-------------------------------------------------------------------------------------------------------------------------------

	enum LinkDestinationType
	{
		destXYZ,
		destFit,
		destFitH,
		destFitV,
		destFitR,
		destFitB,
		destFitBH,
		destFitBV
	};

	class LinkDestination
	{
	public:

		LinkDestination(Array *pArray);

		LinkDestination *Copy()
		{
			return new LinkDestination(this);
		}


		bool CheckValidate()
		{
			return m_bValidate;
		}

		LinkDestinationType GetType()
		{
			return m_eType;
		}
		bool IsPageRef()
		{
			return m_bPageIsRef;
		}
		int GetPageNum()
		{
			return m_nPageNum;
		}
		Ref GetPageRef()
		{
			return m_oPageRef;
		}
		double GetLeft()
		{
			return m_dLeft;
		}
		double GetBottom()
		{
			return m_dBottom;
		}
		double GetRight()
		{
			return m_dRight;
		}
		double GetTop()
		{
			return m_dTop;
		}
		double GetZoom()
		{
			return m_dZoom;
		}
		bool GetChangeLeft()
		{
			return m_bChangeLeft;
		}
		bool GetChangeTop()
		{
			return m_bChangeTop;
		}
		bool GetChangeZoom()
		{
			return m_bChangeZoom;
		}

	private:

		LinkDestination(LinkDestination *pDest);

	private:

		LinkDestinationType m_eType;      // ���
		bool                m_bPageIsRef; // �������� ������ ������� ��� �������?
		union
		{
			Ref m_oPageRef;   // ������ �� ��������
			int m_nPageNum;   // ������������� ����� ��������
		};

		double m_dLeft;       //
		double m_dBottom;     // �������
		double m_dRight;      //
		double m_dTop;        //

		double m_dZoom;       // Zoom

		bool   m_bChangeLeft; // ��� destXYZ. � ������ ������ ����� ����������
		bool   m_bChangeTop;  // ��� �������, ��� � ������������
		bool   m_bChangeZoom; // 

		bool   m_bValidate;   // 

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkGoTo
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkGoTo : public LinkAction
	{
	public:

		LinkGoTo(Object *pDestObject);

		virtual ~LinkGoTo();

		virtual bool CheckValidate()
		{
			return m_pDestination || m_pNamedDestination;
		}

		virtual LinkActionType GetType()
		{
			return actionGoTo;
		}
		LinkDestination *GetDestination()
		{
			return m_pDestination;
		}
		StringExt *GetNamedDestination()
		{
			return m_pNamedDestination;
		}

	private:

		LinkDestination *m_pDestination;
		StringExt       *m_pNamedDestination;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkGoToR
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkGoToR : public LinkAction
	{
	public:

		LinkGoToR(Object *pFileSpecObject, Object *pDestObject);
		virtual ~LinkGoToR();

		virtual bool CheckValidate()
		{
			return m_seFileName && (m_pDestination || m_pNamedDestination);
		}

		virtual LinkActionType GetType()
		{
			return actionGoToR;
		}
		StringExt *GetFileName()
		{
			return m_seFileName;
		}
		LinkDestination *GetDestination()
		{
			return m_pDestination;
		}
		StringExt *GetNamedDestination()
		{
			return m_pNamedDestination;
		}

	private:

		StringExt       *m_seFileName;
		LinkDestination *m_pDestination;
		StringExt       *m_pNamedDestination;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkLaunch
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkLaunch : public LinkAction
	{
	public:

		LinkLaunch(Object *pActionObject);

		virtual ~LinkLaunch();

		virtual bool CheckValidate()
		{
			return m_seFileName != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionLaunch;
		}
		StringExt *GetFileName()
		{
			return m_seFileName;
		}
		StringExt *GetParameters()
		{
			return m_pParameters;
		}

	private:

		StringExt *m_seFileName;
		StringExt *m_pParameters;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkURI
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkURI : public LinkAction
	{
	public:

		LinkURI(Object *pURIObject, StringExt *seBaseURI);

		virtual ~LinkURI();

		virtual bool CheckValidate()
		{
			return m_seURI != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionURI;
		}
		StringExt *GetURI()
		{
			return m_seURI;
		}

	private:

		StringExt *m_seURI;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkNamed
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkNamed : public LinkAction
	{
	public:

		LinkNamed(Object *pNameObject);

		virtual ~LinkNamed();

		virtual bool CheckValidate()
		{
			return m_seName != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionNamed;
		}

		StringExt *GetName()
		{
			return m_seName;
		}

	private:

		StringExt *m_seName;

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkMovie
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkMovie : public LinkAction
	{
	public:

		LinkMovie(Object *pAnnotObject, Object *pTitleObject);

		virtual ~LinkMovie();

		virtual bool CheckValidate()
		{
			return m_oAnnotRef.nNum >= 0 || m_seTitle != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionMovie;
		}

		bool HasAnnotRef()
		{
			return m_oAnnotRef.nNum >= 0;
		}
		Ref *GetAnnotRef()
		{
			return &m_oAnnotRef;
		}
		StringExt *GetTitle()
		{
			return m_seTitle;
		}

	private:

		Ref        m_oAnnotRef;
		StringExt *m_seTitle;

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// LinkUnknown
	//-------------------------------------------------------------------------------------------------------------------------------

	class LinkUnknown : public LinkAction
	{
	public:

		LinkUnknown(char *sAction);

		virtual ~LinkUnknown();

		virtual bool CheckValidate()
		{
			return m_seAction != NULL;
		}

		virtual LinkActionType GetType()
		{
			return actionUnknown;
		}

		StringExt *GetAction()
		{
			return m_seAction;
		}

	private:

		StringExt *m_seAction;

	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Link
	//-------------------------------------------------------------------------------------------------------------------------------

	class Link
	{
	public:

		Link(Dict *pDict, StringExt *seBaseURI);

		~Link();

		bool CheckValidate()
		{
			return m_bValid;
		}

		// ������ �� ����� � ����
		bool InRect(double dX, double dY)
		{
			return m_dLeft <= dX && dX <= m_dRight && m_dBottom <= dY && dY <= m_dTop;
		}

		LinkAction *GetAction()
		{
			return m_pAction;
		}

		void GetRect(double *pdLeft, double *pdBottom, double *pdRight, double *pdTop)
		{
			*pdLeft   = m_dLeft;
			*pdBottom = m_dBottom;
			*pdRight  = m_dRight;
			*pdTop    = m_dTop;
		}

	private:

		double      m_dLeft;
		double      m_dBottom;
		double      m_dRight;
		double      m_dTop;

		LinkAction *m_pAction;
		bool        m_bValid;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// Links
	//-------------------------------------------------------------------------------------------------------------------------------

	class Links
	{
	public:

		Links(Object *pAnnots, StringExt *seBaseURI);

		~Links();

		int GetLinksCount()
		{
			return m_nLinksCount;
		}
		Link *GetLink(int nIndex)
		{
			return m_ppLinks[nIndex];
		}


		// ���� ����� (dX, dY) �������� � Link, ����� ���������� ��������������� Action.
		LinkAction *Find(double dX, double dY);

		// ��������� �������� �� ����� � Link.
		bool OnLink(double dX, double dY);

	private:

		Link **m_ppLinks;
		int    m_nLinksCount;
	};
}

#endif // _PDF_READER_LINK_H
