//------------------------------------------------------------------------------
// Copyright (C) 2016, Pacific Northwest National Laboratory
// This software is subject to copyright protection under the laws of the
// United States and other countries
//
// All rights in this computer software are reserved by the
// Pacific Northwest National Laboratory (PNNL)
// Operated by Battelle for the U.S. Department of Energy
//
//------------------------------------------------------------------------------

// Generated from TAMM.g4 by ANTLR 4.6

#pragma once


#include "TAMMParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by TAMMParser.
 */
class  TAMMVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by TAMMParser.
   */
    virtual antlrcpp::Any visitTranslation_unit(TAMMParser::Translation_unitContext *context) = 0;

    virtual antlrcpp::Any visitCompound_element_list(TAMMParser::Compound_element_listContext *context) = 0;

    virtual antlrcpp::Any visitCompound_element(TAMMParser::Compound_elementContext *context) = 0;

    virtual antlrcpp::Any visitElement_list(TAMMParser::Element_listContext *context) = 0;

    virtual antlrcpp::Any visitElement(TAMMParser::ElementContext *context) = 0;

    virtual antlrcpp::Any visitDeclaration(TAMMParser::DeclarationContext *context) = 0;

    virtual antlrcpp::Any visitScalar_declaration(TAMMParser::Scalar_declarationContext *context) = 0;

    virtual antlrcpp::Any visitId_list_opt(TAMMParser::Id_list_optContext *context) = 0;

    virtual antlrcpp::Any visitId_list(TAMMParser::Id_listContext *context) = 0;

    virtual antlrcpp::Any visitNum_list(TAMMParser::Num_listContext *context) = 0;

    virtual antlrcpp::Any visitIdentifier(TAMMParser::IdentifierContext *context) = 0;

    virtual antlrcpp::Any visitNumerical_constant(TAMMParser::Numerical_constantContext *context) = 0;

    virtual antlrcpp::Any visitRange_declaration(TAMMParser::Range_declarationContext *context) = 0;

    virtual antlrcpp::Any visitIndex_declaration(TAMMParser::Index_declarationContext *context) = 0;

    virtual antlrcpp::Any visitArray_declaration(TAMMParser::Array_declarationContext *context) = 0;

    virtual antlrcpp::Any visitArray_structure(TAMMParser::Array_structureContext *context) = 0;

    virtual antlrcpp::Any visitArray_structure_list(TAMMParser::Array_structure_listContext *context) = 0;

    virtual antlrcpp::Any visitStatement(TAMMParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitAssignment_statement(TAMMParser::Assignment_statementContext *context) = 0;

    virtual antlrcpp::Any visitAssignment_operator(TAMMParser::Assignment_operatorContext *context) = 0;

    virtual antlrcpp::Any visitUnary_expression(TAMMParser::Unary_expressionContext *context) = 0;

    virtual antlrcpp::Any visitArray_reference(TAMMParser::Array_referenceContext *context) = 0;

    virtual antlrcpp::Any visitPlusORminus(TAMMParser::PlusORminusContext *context) = 0;

    virtual antlrcpp::Any visitExpression(TAMMParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicative_expression(TAMMParser::Multiplicative_expressionContext *context) = 0;


};

