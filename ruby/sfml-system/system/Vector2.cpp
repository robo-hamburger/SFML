#include "Vector2.hpp"
#include "System.hpp"

/* SFML::Vector2 is a simple class that defines a mathematical vector with two coordinates (x and y).
 *
 * It can be used to represent anything that has two dimensions: a size, a point, a velocity, etc.
 *
 * This class differs from the C++ version. It will accept any value that is Numeric and both x and y must be of the same class.
 *
 *   v1 = SFML::Vector2.new(16.5, 24.0)
 *   v1.x = 18.2
 *   y = v1.y
 *
 *   v2 = v1 * v1;
 *   v3 = SFML::Vector2.new
 *   v3 = v1 + v2
 *
 *   different = (v2 != v3);
 */
VALUE globalVector2Class;

/* Internal function
 * Forces the argument someValue to be a Vector2. IF it can convert it then it will.
 * So you can always safely asume that this function returns a Vector2 object.
 * If it fails then an exception will be thrown.
 */
VALUE Vector2_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == true )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, arg1, arg2 );
	}
	else if( rb_obj_is_kind_of( someValue, globalVector2Class ) == true )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Vector2" );
	}
}

/* Internal function
 * Will copy the x and y from aSource to self.
 */
static void Vector2_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE vectorSource = Vector2_ForceType( aSource );
	VALUE x = rb_funcall( vectorSource, rb_intern( "x" ), 0 );
	VALUE y = rb_funcall( vectorSource, rb_intern( "y" ), 0 );
	
	rb_funcall( self, rb_intern( "x=" ), 1, x ); 
	rb_funcall( self, rb_intern( "y=" ), 1, y );
	rb_iv_set( self, "@dataType", rb_iv_get( vectorSource, "@dataType" ) );
}

/* Internal function
 * Validate that the passed types are the same and numeric.
 */
static void Vector2_internal_ValidateTypes( VALUE aFirst, VALUE aSecond )
{
	if( CLASS_OF( aFirst ) != CLASS_OF( aSecond ) )
	{
		rb_raise( rb_eRuntimeError, "x and y must be of same type" );
	}
	
	if( rb_obj_is_kind_of( aFirst, rb_cNumeric ) == Qfalse )
	{
		rb_raise( rb_eRuntimeError, "x and y must be numeric!" );
	}
}

static VALUE Vector2_Negate( VALUE self )
{
	VALUE x = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE y = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE negatedX = rb_funcall( x, rb_intern( "-@" ), 0 );
	VALUE negatedY = rb_funcall( y, rb_intern( "-@" ), 0 );
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, negatedX, negatedY );
}

static VALUE Vector2_Add( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "+" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "+" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Subtract( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation
	VALUE newX = rb_funcall( leftX, rb_intern( "-" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "-" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Multiply( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation
	VALUE newX = rb_funcall( leftX, rb_intern( "*" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "*" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Divide( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "/" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "/" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Equal( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector2_ForceType( anArgument );
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( aVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( aVector, rb_intern( "y" ), 0 );
	
	if( rb_funcall( leftX, rb_intern( "==" ), 1, rightX ) == Qtrue &&
	    rb_funcall( leftY, rb_intern( "==" ), 1, rightY ) == Qtrue )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Vector2_StrictEqual( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector2_ForceType( anArgument );
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( aVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( aVector, rb_intern( "y" ), 0 );
	
	if( rb_funcall( leftX, rb_intern( "eql?" ), 1, rightX ) == Qtrue &&
	    rb_funcall( leftY, rb_intern( "eql?" ), 1, rightY ) == Qtrue )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   Vector2.new() 			-> vector
 *   Vector2.new([x,y])		-> vector
 *	 Vector2.new(vector) 	-> vector
 *   Vector2.new(x,y)		-> vector
 * 
 * Create a new vector instance.
 */
static VALUE Vector2_Initialize( VALUE self, VALUE someArgs )
{
	long arrayLength = RARRAY_LEN( someArgs );
	rb_iv_set( self, "@x", INT2NUM( 0 ) );
	rb_iv_set( self, "@y", INT2NUM( 0 ) );
	
	if( arrayLength == 0 )
	{
		// Nothing needs to be done
	}
	else if( arrayLength == 1 )
	{
		Vector2_internal_CopyFrom( self, rb_ary_entry( someArgs, 0 ) );
	}
	else if( arrayLength == 2 )
	{
		VALUE arg1 = rb_ary_entry( someArgs, 0 );
		VALUE arg2 = rb_ary_entry( someArgs, 1 );
		Vector2_internal_ValidateTypes( arg1, arg2 );
		
		rb_iv_set( self, "@x", arg1 );
		rb_iv_set( self, "@y", arg2 );
	}
	
	rb_iv_set( self, "@dataType", CLASS_OF( rb_iv_get( self, "@x" ) ) );
	return self;
}

VALUE Vector2_New( int anArgCount, VALUE * someArgs, VALUE aKlass )
{
	return rb_call_super( anArgCount, someArgs );
}

void Init_Vector2( void )
{
	globalVector2Class = rb_define_class_under( GetNamespace(), "Vector2", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalVector2Class, "new", FUNCPTR( Vector2_New ), -1 );
	
	// Instance methods
	rb_define_method( globalVector2Class, "initialize", FUNCPTR( Vector2_Initialize ), -2 );
	rb_define_method( globalVector2Class, "eql?", FUNCPTR( Vector2_Initialize ), 1 );
	
	// Instance operators
	rb_define_method( globalVector2Class, "-@", FUNCPTR( Vector2_Negate ), 0 );
	rb_define_method( globalVector2Class, "+", FUNCPTR( Vector2_Add ), 1 );
	rb_define_method( globalVector2Class, "-", FUNCPTR( Vector2_Subtract ), 1 );
	rb_define_method( globalVector2Class, "*", FUNCPTR( Vector2_Multiply ), 1 );
	rb_define_method( globalVector2Class, "/", FUNCPTR( Vector2_Divide ), 1 );
	rb_define_method( globalVector2Class, "==", FUNCPTR( Vector2_Divide ), 1 );
	
	// Attribute accessors
	rb_define_attr( globalVector2Class, "x", 1, 1 );
	rb_define_attr( globalVector2Class, "y", 1, 1 );
}
