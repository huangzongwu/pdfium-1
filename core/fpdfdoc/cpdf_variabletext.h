// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFDOC_CPDF_VARIABLETEXT_H_
#define CORE_FPDFDOC_CPDF_VARIABLETEXT_H_

#include <memory>

#include "core/fpdfdoc/cpvt_arraytemplate.h"
#include "core/fpdfdoc/cpvt_floatrect.h"
#include "core/fpdfdoc/cpvt_line.h"
#include "core/fpdfdoc/cpvt_lineinfo.h"
#include "core/fpdfdoc/cpvt_wordplace.h"
#include "core/fpdfdoc/cpvt_wordrange.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_string.h"
#include "core/fxcrt/fx_system.h"
#include "core/fxge/fx_font.h"

class CSection;
class IPVT_FontMap;

struct CPVT_SecProps;
struct CPVT_Section;
struct CPVT_SectionInfo;
struct CPVT_Word;
struct CPVT_WordInfo;
struct CPVT_WordProps;

#define VARIABLETEXT_HALF 0.5f

class CPDF_VariableText {
 public:
  enum class ScriptType { Normal, Super, Sub };

  class Iterator {
   public:
    explicit Iterator(CPDF_VariableText* pVT);
    ~Iterator();

    FX_BOOL NextWord();
    FX_BOOL PrevWord();
    FX_BOOL NextLine();
    FX_BOOL PrevLine();
    FX_BOOL NextSection();
    FX_BOOL PrevSection();
    FX_BOOL SetWord(const CPVT_Word& word);
    FX_BOOL GetWord(CPVT_Word& word) const;
    FX_BOOL GetLine(CPVT_Line& line) const;
    FX_BOOL GetSection(CPVT_Section& section) const;
    FX_BOOL SetSection(const CPVT_Section& section);
    void SetAt(int32_t nWordIndex);
    void SetAt(const CPVT_WordPlace& place);
    const CPVT_WordPlace& GetAt() const { return m_CurPos; }

   private:
    CPVT_WordPlace m_CurPos;
    CPDF_VariableText* const m_pVT;
  };

  class Provider {
   public:
    explicit Provider(IPVT_FontMap* pFontMap);
    virtual ~Provider();

    virtual int32_t GetCharWidth(int32_t nFontIndex, uint16_t word);
    virtual int32_t GetTypeAscent(int32_t nFontIndex);
    virtual int32_t GetTypeDescent(int32_t nFontIndex);
    virtual int32_t GetWordFontIndex(uint16_t word,
                                     int32_t charset,
                                     int32_t nFontIndex);
    virtual FX_BOOL IsLatinWord(uint16_t word);
    virtual int32_t GetDefaultFontIndex();

   private:
    IPVT_FontMap* const m_pFontMap;
  };

  CPDF_VariableText();
  ~CPDF_VariableText();

  void SetProvider(CPDF_VariableText::Provider* pProvider);
  CPDF_VariableText::Iterator* GetIterator();

  void SetContentRect(const CPVT_FloatRect& rect);
  CFX_FloatRect GetContentRect() const;
  void SetPlateRect(const CFX_FloatRect& rect);
  const CFX_FloatRect& GetPlateRect() const;

  void SetAlignment(int32_t nFormat) { m_nAlignment = nFormat; }
  void SetPasswordChar(uint16_t wSubWord) { m_wSubWord = wSubWord; }
  void SetLimitChar(int32_t nLimitChar) { m_nLimitChar = nLimitChar; }
  void SetCharSpace(FX_FLOAT fCharSpace) { m_fCharSpace = fCharSpace; }
  void SetMultiLine(FX_BOOL bMultiLine) { m_bMultiLine = bMultiLine; }
  void SetAutoReturn(FX_BOOL bAuto) { m_bLimitWidth = bAuto; }
  void SetFontSize(FX_FLOAT fFontSize) { m_fFontSize = fFontSize; }
  void SetCharArray(int32_t nCharArray) { m_nCharArray = nCharArray; }
  void SetAutoFontSize(FX_BOOL bAuto) { m_bAutoFontSize = bAuto; }
  void Initialize();

  FX_BOOL IsValid() const { return m_bInitial; }

  void RearrangeAll();
  void RearrangePart(const CPVT_WordRange& PlaceRange);
  void ResetAll();
  void SetText(const CFX_WideString& text);
  CPVT_WordPlace InsertWord(const CPVT_WordPlace& place,
                            uint16_t word,
                            int32_t charset,
                            const CPVT_WordProps* pWordProps);
  CPVT_WordPlace InsertSection(const CPVT_WordPlace& place,
                               const CPVT_SecProps* pSecProps,
                               const CPVT_WordProps* pWordProps);
  CPVT_WordPlace InsertText(const CPVT_WordPlace& place, const FX_WCHAR* text);
  CPVT_WordPlace DeleteWords(const CPVT_WordRange& PlaceRange);
  CPVT_WordPlace DeleteWord(const CPVT_WordPlace& place);
  CPVT_WordPlace BackSpaceWord(const CPVT_WordPlace& place);

  int32_t GetTotalWords() const;
  FX_FLOAT GetFontSize() const { return m_fFontSize; }
  int32_t GetAlignment() const { return m_nAlignment; }
  uint16_t GetPasswordChar() const { return GetSubWord(); }
  int32_t GetCharArray() const { return m_nCharArray; }
  int32_t GetLimitChar() const { return m_nLimitChar; }
  FX_BOOL IsMultiLine() const { return m_bMultiLine; }
  int32_t GetHorzScale() const { return m_nHorzScale; }
  FX_FLOAT GetCharSpace() const { return m_fCharSpace; }
  CPVT_WordPlace GetBeginWordPlace() const;
  CPVT_WordPlace GetEndWordPlace() const;
  CPVT_WordPlace GetPrevWordPlace(const CPVT_WordPlace& place) const;
  CPVT_WordPlace GetNextWordPlace(const CPVT_WordPlace& place) const;
  CPVT_WordPlace SearchWordPlace(const CFX_FloatPoint& point) const;
  CPVT_WordPlace GetUpWordPlace(const CPVT_WordPlace& place,
                                const CFX_FloatPoint& point) const;
  CPVT_WordPlace GetDownWordPlace(const CPVT_WordPlace& place,
                                  const CFX_FloatPoint& point) const;
  CPVT_WordPlace GetLineBeginPlace(const CPVT_WordPlace& place) const;
  CPVT_WordPlace GetLineEndPlace(const CPVT_WordPlace& place) const;
  CPVT_WordPlace GetSectionBeginPlace(const CPVT_WordPlace& place) const;
  CPVT_WordPlace GetSectionEndPlace(const CPVT_WordPlace& place) const;
  void UpdateWordPlace(CPVT_WordPlace& place) const;
  CPVT_WordPlace AdjustLineHeader(const CPVT_WordPlace& place,
                                  FX_BOOL bPrevOrNext) const;
  int32_t WordPlaceToWordIndex(const CPVT_WordPlace& place) const;
  CPVT_WordPlace WordIndexToWordPlace(int32_t index) const;

  uint16_t GetSubWord() const { return m_wSubWord; }

  FX_FLOAT GetPlateWidth() const { return m_rcPlate.right - m_rcPlate.left; }
  FX_FLOAT GetPlateHeight() const { return m_rcPlate.top - m_rcPlate.bottom; }
  CFX_SizeF GetPlateSize() const;
  CFX_FloatPoint GetBTPoint() const;
  CFX_FloatPoint GetETPoint() const;

  CFX_FloatPoint InToOut(const CFX_FloatPoint& point) const;
  CFX_FloatPoint OutToIn(const CFX_FloatPoint& point) const;
  CFX_FloatRect InToOut(const CPVT_FloatRect& rect) const;
  CPVT_FloatRect OutToIn(const CFX_FloatRect& rect) const;

 private:
  friend class CTypeset;
  friend class CSection;

  int32_t GetCharWidth(int32_t nFontIndex, uint16_t Word, uint16_t SubWord);
  int32_t GetTypeAscent(int32_t nFontIndex);
  int32_t GetTypeDescent(int32_t nFontIndex);
  int32_t GetWordFontIndex(uint16_t word, int32_t charset, int32_t nFontIndex);
  int32_t GetDefaultFontIndex();
  FX_BOOL IsLatinWord(uint16_t word);

  CPVT_WordPlace AddSection(const CPVT_WordPlace& place,
                            const CPVT_SectionInfo& secinfo);
  CPVT_WordPlace AddLine(const CPVT_WordPlace& place,
                         const CPVT_LineInfo& lineinfo);
  CPVT_WordPlace AddWord(const CPVT_WordPlace& place,
                         const CPVT_WordInfo& wordinfo);
  FX_BOOL GetWordInfo(const CPVT_WordPlace& place, CPVT_WordInfo& wordinfo);
  FX_BOOL SetWordInfo(const CPVT_WordPlace& place,
                      const CPVT_WordInfo& wordinfo);
  FX_BOOL GetLineInfo(const CPVT_WordPlace& place, CPVT_LineInfo& lineinfo);
  FX_BOOL GetSectionInfo(const CPVT_WordPlace& place,
                         CPVT_SectionInfo& secinfo);
  FX_FLOAT GetWordFontSize(const CPVT_WordInfo& WordInfo);
  FX_FLOAT GetWordWidth(int32_t nFontIndex,
                        uint16_t Word,
                        uint16_t SubWord,
                        FX_FLOAT fCharSpace,
                        int32_t nHorzScale,
                        FX_FLOAT fFontSize,
                        FX_FLOAT fWordTail);
  FX_FLOAT GetWordWidth(const CPVT_WordInfo& WordInfo);
  FX_FLOAT GetWordAscent(const CPVT_WordInfo& WordInfo, FX_FLOAT fFontSize);
  FX_FLOAT GetWordDescent(const CPVT_WordInfo& WordInfo, FX_FLOAT fFontSize);
  FX_FLOAT GetWordAscent(const CPVT_WordInfo& WordInfo);
  FX_FLOAT GetWordDescent(const CPVT_WordInfo& WordInfo);
  FX_FLOAT GetLineAscent(const CPVT_SectionInfo& SecInfo);
  FX_FLOAT GetLineDescent(const CPVT_SectionInfo& SecInfo);
  FX_FLOAT GetFontAscent(int32_t nFontIndex, FX_FLOAT fFontSize);
  FX_FLOAT GetFontDescent(int32_t nFontIndex, FX_FLOAT fFontSize);
  int32_t GetWordFontIndex(const CPVT_WordInfo& WordInfo);
  FX_FLOAT GetCharSpace(const CPVT_WordInfo& WordInfo);
  int32_t GetHorzScale(const CPVT_WordInfo& WordInfo);
  FX_FLOAT GetLineLeading(const CPVT_SectionInfo& SecInfo);
  FX_FLOAT GetLineIndent(const CPVT_SectionInfo& SecInfo);
  int32_t GetAlignment(const CPVT_SectionInfo& SecInfo);

  void ClearSectionRightWords(const CPVT_WordPlace& place);

  FX_BOOL ClearEmptySection(const CPVT_WordPlace& place);
  void ClearEmptySections(const CPVT_WordRange& PlaceRange);
  void LinkLatterSection(const CPVT_WordPlace& place);
  void ClearWords(const CPVT_WordRange& PlaceRange);
  CPVT_WordPlace ClearLeftWord(const CPVT_WordPlace& place);
  CPVT_WordPlace ClearRightWord(const CPVT_WordPlace& place);

  CPVT_FloatRect Rearrange(const CPVT_WordRange& PlaceRange);
  FX_FLOAT GetAutoFontSize();
  bool IsBigger(FX_FLOAT fFontSize) const;
  CPVT_FloatRect RearrangeSections(const CPVT_WordRange& PlaceRange);

  void ResetSectionArray();

  CPVT_ArrayTemplate<CSection*> m_SectionArray;
  int32_t m_nLimitChar;
  int32_t m_nCharArray;
  FX_BOOL m_bMultiLine;
  FX_BOOL m_bLimitWidth;
  FX_BOOL m_bAutoFontSize;
  int32_t m_nAlignment;
  FX_FLOAT m_fLineLeading;
  FX_FLOAT m_fCharSpace;
  int32_t m_nHorzScale;
  uint16_t m_wSubWord;
  FX_FLOAT m_fFontSize;
  FX_BOOL m_bInitial;
  CPDF_VariableText::Provider* m_pVTProvider;
  std::unique_ptr<CPDF_VariableText::Iterator> m_pVTIterator;
  CFX_FloatRect m_rcPlate;
  CPVT_FloatRect m_rcContent;
};

#endif  // CORE_FPDFDOC_CPDF_VARIABLETEXT_H_
