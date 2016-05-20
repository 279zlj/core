#ifndef _CPDOCCORE_VISITOR_H_INCLUDED_
#define _CPDOCCORE_VISITOR_H_INCLUDED_

/*

����� ������ � visitor:

��� �������, ������� ����� ���� �������� visitor-��:
1. ��������� �� base_visitable
2. ��������� ������ ������ CPDOCCORE_DEFINE_VISITABLE()

class example_visitable_element: public base_visitable 
{
// ... 
public:
    CPDOCCORE_DEFINE_VISITABLE();
// ... 
};

��� �������� �����:
1. ��������� ����� �� base_visitor
2. ��������� �����(�������) �� const_visitor<visitable_element1>/visitor<visitable_element1>, 
                        const_visitor<visitable_element2>/visitor<visitable_element2>, � �.�.
   ��� ������� �� ���������, ������� ����� �������� �������
3. ��������� ��� ������� �� ��������� ����� 
    virtual void visit(const visitable_element1& val);
    virtual void visit(const visitable_element2& val);
    � �.�.
    (� ������ �������������� - ������� const)


class table_round : public base_visitor, 
    public const_visitor<office_body>,
    public const_visitor<office_spreadsheet>,
    public const_visitor<table_table>
{
    virtual void visit(const office_body& val)
    {
    // ... 
    }

    virtual void visit(const office_spreadsheet& val)
    {
    // ...
    }

    virtual void visit(const table_table& val)
    {
    // ... 
    }
};

��� ����������� ������:
1. ������� �������
2. ��������� accept ��� ��������� ��������

office_element * root;
/// ... 
table_round tableRoundVisitor;
root->accept(tableRoundVisitor);

��������! ������� ��� ���������� ������� ������ ���������� ������, �.�. ������ ������ ������� ��������� ��������� �������� ��������
    
    virtual void visit(const office_body& val)
    {
        val.content_->accept(*this);        
    }

*/

namespace cpdoccore { 
    
    class base_visitor
    {
    public:
        virtual ~base_visitor() {}    
        virtual void on_not_impl(std::string const & message) = 0;
    };

    template <class T>
    class visitor
    {
    public:
        virtual void visit(T&) = 0;
       // virtual void visit(T const& t) = 0;
    };

    template <class T>
    class const_visitor
    {
    public:
        virtual void visit(T const& t) = 0;
    };


    class base_visitable
    {
    public:
        virtual ~base_visitable() {};
        virtual void accept(base_visitor &) = 0;
        virtual void accept(base_visitor &) const = 0;
    protected:
        template <class T>
        static void accept_impl(T& visited, base_visitor & guest)
        {
            if (visitor<T> * p = dynamic_cast< visitor<T> * >(&guest))
            {
                return p->visit(visited);
            }
            else
            {
                guest.on_not_impl(typeid(visited).name());
            }
        }

        template <class T>
        static void accept_impl(T const& visited, base_visitor & guest)
        {
            if (const_visitor<T> * p = dynamic_cast< const_visitor<T> * >(&guest))
            {
                return p->visit(visited);
            }
            else
            {
                guest.on_not_impl(typeid(visited).name());
            }
        }
    };


#define CPDOCCORE_DEFINE_VISITABLE() \
    virtual void accept(base_visitor& guest)\
    {\
        return accept_impl(*this, guest);\
    }\
    virtual void accept(base_visitor& guest) const\
    {\
        return accept_impl(*this, guest);\
    }

}

#endif
