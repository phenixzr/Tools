/**
  None type means void parameters or return value.
*/
typedef void None;

template <typename Head, typename Tail = None>
struct TypeList
{
};

/**
  A TypeList with actual values.
*/
template <typename List>
struct TypeListValues
{
  static std::string const tostring (bool)
  {
    return "";
  }
};

/**
  TypeListValues recursive template definition.
*/
template <typename Head, typename Tail>
struct TypeListValues <TypeList <Head, Tail> >
{
  Head hd;
  TypeListValues <Tail> tl;

  TypeListValues (Head hd_, TypeListValues <Tail> const& tl_)
    : hd (hd_), tl (tl_)
  {
  }

  static std::string const tostring (bool comma = false)
  {
    std::string s;

    if (comma)
      s = ", ";

    s = s + typeid (Head).name ();

    return s + TypeListValues <Tail>::tostring (true);
  }
};

// Specializations of type/value list for head types that are references and
// const-references.  We need to handle these specially since we can't count
// on the referenced object hanging around for the lifetime of the list.

template <typename Head, typename Tail>
struct TypeListValues <TypeList <Head&, Tail> >
{
  Head hd;
  TypeListValues <Tail> tl;

  TypeListValues (Head& hd_, TypeListValues <Tail> const& tl_)
    : hd (hd_), tl (tl_)
  {
  }

  static std::string const tostring (bool comma = false)
  {
    std::string s;

    if (comma)
      s = ", ";

    s = s + typeid (Head).name () + "&";

    return s + TypeListValues <Tail>::tostring (true);
  }
};

template <typename Head, typename Tail>
struct TypeListValues <TypeList <Head const&, Tail> >
{
  Head hd;
  TypeListValues <Tail> tl;

  TypeListValues (Head const& hd_, const TypeListValues <Tail>& tl_)
    : hd (hd_), tl (tl_)
  {
  }

  static std::string const tostring (bool comma = false)
  {
    std::string s;

    if (comma)
      s = ", ";

    s = s + typeid (Head).name () + " const&";

    return s + TypeListValues <Tail>::tostring (true);
  }
};

//==============================================================================
/**
  Subclass of a TypeListValues constructable from the Lua stack.
*/

template <typename List, int Start = 1>
struct ArgList
{
};

template <int Start>
struct ArgList <None, Start> : public TypeListValues <None>
{
  ArgList (lua_State*)
  {
  }
};

template <typename Head, typename Tail, int Start>
struct ArgList <TypeList <Head, Tail>, Start>
  : public TypeListValues <TypeList <Head, Tail> >
{
  ArgList (lua_State* L)
    : TypeListValues <TypeList <Head, Tail> > (Stack <Head>::get (L, Start),
                                            ArgList <Tail, Start + 1> (L))
  {
  }
};
